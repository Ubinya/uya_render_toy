#pragma once

#include "dx12Common/d3dApp.h"
#include "dx12Common/d3dUtil.h"
#include "dx12Common/MathHelper.h"
#include "dx12Common/UploadBuffer.h"

#include "TexShape.h"

using Microsoft::WRL::ComPtr;
using namespace DirectX;
using namespace DirectX::PackedVector;

const int gNumFrameResources = 3;

TexShape::TexShape(HINSTANCE hInstance) 
    : D3DApp(hInstance){

}

TexShape::~TexShape() {
    if (md3dDevice != nullptr)
        FlushCommandQueue();
}


void TexShape::Update(const GameTimer& gt) {

}

void TexShape::Draw(const GameTimer& gt) {

}

void TexShape::OnResize()
{
    D3DApp::OnResize();

    // The window resized, so update the aspect ratio and recompute the projection matrix.
    XMMATRIX P = XMMatrixPerspectiveFovLH(0.25f * MathHelper::Pi, AspectRatio(), 1.0f, 1000.0f);
    XMStoreFloat4x4(&mProj, P);
}

bool TexShape::Initialize() {
    if (!D3DApp::Initialize())
        return false;

    // Reset the command list to prep for initialization commands.
    ThrowIfFailed(mCommandList->Reset(mDirectCmdListAlloc.Get(), nullptr));

    BuildRootSignature();
    BuildShadersAndInputLayout();
    BuildTexShapeGeometry();
    BuildRenderItems();
    BuildFrameResources();
    BuildDescriptorHeaps();
    BuildConstantBufferViews();
    BuildShaderResourceViews();
    BuildPSOs();

    // Execute the initialization commands.
    ThrowIfFailed(mCommandList->Close());
    ID3D12CommandList* cmdsLists[] = { mCommandList.Get() };
    mCommandQueue->ExecuteCommandLists(_countof(cmdsLists), cmdsLists);

    // Wait until initialization is complete.
    FlushCommandQueue();

    return true;
}

void TexShape::BuildTexShapeGeometry() {
    //----setting texture info
    auto woodCrateTex = std::make_unique<Texture>();
    woodCrateTex->Name = "T_companion_cube";
    woodCrateTex->Filename = L"asserts/companion_cube.dds";
    ThrowIfFailed(DirectX::CreateDDSTextureFromFile12(
        md3dDevice.Get(), mCommandList.Get(),
        woodCrateTex->Filename.c_str(),
        woodCrateTex->Resource, woodCrateTex->UploadHeap));


}

void TexShape::BuildDescriptorHeaps() {
    D3D12_DESCRIPTOR_HEAP_DESC srvHeapDesc = { };
    srvHeapDesc.NumDescriptors = 3;
    srvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
    srvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
    ThrowIfFailed(md3dDevice->CreateDescriptorHeap(
        &srvHeapDesc, IID_PPV_ARGS(&mSrvDescriptorHeap)));

}