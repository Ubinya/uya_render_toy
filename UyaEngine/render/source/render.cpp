#include "dx12Common/d3dApp.h"

#include "render/include/render/render.h"

using namespace uyatoy;
using namespace DirectX;
using namespace std;
using Microsoft::WRL::ComPtr;

namespace uyatoy {
	static inline CD3DX12_RESOURCE_DESC Buffer(
		UINT64 width,
		D3D12_RESOURCE_FLAGS flags = D3D12_RESOURCE_FLAG_NONE,
		UINT64 alignment = 0)
	{
		return CD3DX12_RESOURCE_DESC(D3D12_RESOURCE_DIMENSION_BUFFER,
			alignment, width, 1, 1, 1,
			DXGI_FORMAT_UNKNOWN, 1, 0,
			D3D12_TEXTURE_LAYOUT_ROW_MAJOR, flags);
	}



	
	void Renderer::UpdateObjectCBs(const GameTimer& gt) {
		auto currObjectCB = mCurrFrameResource->ObjectCB.get();
		for (auto& e : mAllRitems)
		{
			// Only update the cbuffer data if the constants have changed.  
			// This needs to be tracked per frame resource.
			if (e->NumFramesDirty > 0)
			{
				XMMATRIX world = XMLoadFloat4x4(&e->World);

				ObjectConstants objConstants;
				XMStoreFloat4x4(&objConstants.World, XMMatrixTranspose(world));

				currObjectCB->CopyData(e->ObjCBIndex, objConstants);

				// Next FrameResource need to be updated too.
				e->NumFramesDirty--;
			}
		}
	}

	void Renderer::Update(const GameTimer& gt) {
		
        // Cycle through the circular frame resource array.
        mCurrFrameResourceIndex = (mCurrFrameResourceIndex + 1) % gNumFrameResources;
        mCurrFrameResource = mFrameResources[mCurrFrameResourceIndex].get();

        // Has the GPU finished processing the commands of the current frame resource?
        // If not, wait until the GPU has completed commands up to this fence point.
        if (mCurrFrameResource->Fence != 0 && mFence->GetCompletedValue() < mCurrFrameResource->Fence)
        {
            HANDLE eventHandle = CreateEventEx(nullptr, false, false, EVENT_ALL_ACCESS);
            ThrowIfFailed(mFence->SetEventOnCompletion(mCurrFrameResource->Fence, eventHandle));
            WaitForSingleObject(eventHandle, INFINITE);
            CloseHandle(eventHandle);
        }

        UpdateObjectCBs(gt);
        UpdateMainPassCB(gt);
	}

	void Renderer::Draw(const GameTimer& gt) {
        //从当前帧资源分配一个新cmd列表
        auto cmdListAlloc = mCurrFrameResource->CmdListAlloc;

        // Reuse the memory associated with command recording.
        // We can only reset when the associated command lists have finished execution on the GPU.
        ThrowIfFailed(cmdListAlloc->Reset());

        // A command list can be reset after it has been added to the command queue via ExecuteCommandList.
        // Reusing the command list reuses memory.
        //把新的cmd列表赋给cmd列表对象，以及更新管线状态对象（PSO）
        if (mIsWireframe)
        {
            ThrowIfFailed(mCommandList->Reset(cmdListAlloc.Get(), mPSOs["opaque_wireframe"].Get()));
        }
        else
        {
            ThrowIfFailed(mCommandList->Reset(cmdListAlloc.Get(), mPSOs["opaque"].Get()));
        }
        //开始给新cmd列表添加新命令，设置新的viewport和裁剪区域
        mCommandList->RSSetViewports(1, &mScreenViewport);
        mCommandList->RSSetScissorRects(1, &mScissorRect);

        // Indicate a state transition on the resource usage.
        mCommandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(CurrentBackBuffer(),
            D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET));

        // Clear the back buffer and depth buffer.
        mCommandList->ClearRenderTargetView(CurrentBackBufferView(), Colors::LightSteelBlue, 0, nullptr);
        mCommandList->ClearDepthStencilView(DepthStencilView(), D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL, 1.0f, 0, 0, nullptr);

        // Specify the buffers we are going to render to.
        mCommandList->OMSetRenderTargets(1, &CurrentBackBufferView(), true, &DepthStencilView());

        ID3D12DescriptorHeap* descriptorHeaps[] = { mCbvHeap.Get() };
        mCommandList->SetDescriptorHeaps(_countof(descriptorHeaps), descriptorHeaps);

        mCommandList->SetGraphicsRootSignature(mRootSignature.Get());

        int passCbvIndex = mPassCbvOffset + mCurrFrameResourceIndex;
        auto passCbvHandle = CD3DX12_GPU_DESCRIPTOR_HANDLE(mCbvHeap->GetGPUDescriptorHandleForHeapStart());
        passCbvHandle.Offset(passCbvIndex, mCbvSrvUavDescriptorSize);
        mCommandList->SetGraphicsRootDescriptorTable(1, passCbvHandle);

        DrawRenderItems(mCommandList.Get(), mOpaqueRitems);

        // Indicate a state transition on the resource usage.
        mCommandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(CurrentBackBuffer(),
            D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT));

        // Done recording commands.
        ThrowIfFailed(mCommandList->Close());

        // Add the command list to the queue for execution.
        ID3D12CommandList* cmdsLists[] = { mCommandList.Get() };
        mCommandQueue->ExecuteCommandLists(_countof(cmdsLists), cmdsLists);

        // Swap the back and front buffers
        ThrowIfFailed(mSwapChain->Present(0, 0));
        mCurrBackBuffer = (mCurrBackBuffer + 1) % SwapChainBufferCount;

        // Advance the fence value to mark commands up to this fence point.
        mCurrFrameResource->Fence = ++mCurrentFence;

        // Add an instruction to the command queue to set a new fence point. 
        // Because we are on the GPU timeline, the new fence point won't be 
        // set until the GPU finishes processing all the commands prior to this Signal().
        mCommandQueue->Signal(mFence.Get(), mCurrentFence);
	}

    void Renderer::DrawRenderItems(ID3D12GraphicsCommandList* cmdList, const std::vector<RenderItem*>& ritems)
    {
        UINT objCBByteSize = d3dUtil::CalcConstantBufferByteSize(sizeof(ObjectConstants));

        auto objectCB = mCurrFrameResource->ObjectCB->Resource();

        // For each render item...
        for (size_t i = 0; i < ritems.size(); ++i)
        {
            auto ri = ritems[i];

            cmdList->IASetVertexBuffers(0, 1, &ri->Geo->VertexBufferView());
            cmdList->IASetIndexBuffer(&ri->Geo->IndexBufferView());
            cmdList->IASetPrimitiveTopology(ri->PrimitiveType);

            // Offset to the CBV in the descriptor heap for this object and for this frame resource.
            UINT cbvIndex = mCurrFrameResourceIndex * (UINT)mOpaqueRitems.size() + ri->ObjCBIndex;
            auto cbvHandle = CD3DX12_GPU_DESCRIPTOR_HANDLE(mCbvHeap->GetGPUDescriptorHandleForHeapStart());
            cbvHandle.Offset(cbvIndex, mCbvSrvUavDescriptorSize);

            cmdList->SetGraphicsRootDescriptorTable(0, cbvHandle);

            cmdList->DrawIndexedInstanced(ri->IndexCount, 1, ri->StartIndexLocation, ri->BaseVertexLocation, 0);
        }
    }

    void Renderer::UpdateMainPassCB(const GameTimer& gt)
    {
        XMMATRIX view = XMLoadFloat4x4(&mView);
        XMMATRIX proj = XMLoadFloat4x4(&mProj);

        XMMATRIX viewProj = XMMatrixMultiply(view, proj);
        XMMATRIX invView = XMMatrixInverse(&XMMatrixDeterminant(view), view);
        XMMATRIX invProj = XMMatrixInverse(&XMMatrixDeterminant(proj), proj);
        XMMATRIX invViewProj = XMMatrixInverse(&XMMatrixDeterminant(viewProj), viewProj);

        XMStoreFloat4x4(&mMainPassCB.View, XMMatrixTranspose(view));
        XMStoreFloat4x4(&mMainPassCB.InvView, XMMatrixTranspose(invView));
        XMStoreFloat4x4(&mMainPassCB.Proj, XMMatrixTranspose(proj));
        XMStoreFloat4x4(&mMainPassCB.InvProj, XMMatrixTranspose(invProj));
        XMStoreFloat4x4(&mMainPassCB.ViewProj, XMMatrixTranspose(viewProj));
        XMStoreFloat4x4(&mMainPassCB.InvViewProj, XMMatrixTranspose(invViewProj));
        mMainPassCB.EyePosW = mEyePos;
        mMainPassCB.RenderTargetSize = XMFLOAT2((float)mClientWidth, (float)mClientHeight);
        mMainPassCB.InvRenderTargetSize = XMFLOAT2(1.0f / mClientWidth, 1.0f / mClientHeight);
        mMainPassCB.NearZ = 1.0f;
        mMainPassCB.FarZ = 1000.0f;
        mMainPassCB.TotalTime = gt.TotalTime();
        mMainPassCB.DeltaTime = gt.DeltaTime();

        auto currPassCB = mCurrFrameResource->PassCB.get();
        currPassCB->CopyData(0, mMainPassCB);
    }

}