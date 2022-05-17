#include <string>

#include "dx12Common/d3dApp.h"
#include "dx12Common/d3dUtil.h"

#include "function/data/dataloader.h"

using namespace std;
using namespace DirectX;

namespace uyatoy {
	DataLoader::DataLoader(Microsoft::WRL::ComPtr<ID3D12Device> d3dDevice,
		Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> CommandList){
		md3dDevice = d3dDevice;
		mCommandList = CommandList;
	}
	DataLoader::~DataLoader() {}

	std::unique_ptr<MeshGeometry> DataLoader::BuildSphereGeometry(XMFLOAT3 center, float rad) {

	}

	std::unique_ptr<MeshGeometry> DataLoader::BuildBoxGeometry()
	{
		std::array<Vertex, 8> vertices =
		{
			Vertex({ XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT4(Colors::White) }),
			Vertex({ XMFLOAT3(-1.0f, +1.0f, -1.0f), XMFLOAT4(Colors::Black) }),
			Vertex({ XMFLOAT3(+1.0f, +1.0f, -1.0f), XMFLOAT4(Colors::Red) }),
			Vertex({ XMFLOAT3(+1.0f, -1.0f, -1.0f), XMFLOAT4(Colors::Green) }),
			Vertex({ XMFLOAT3(-1.0f, -1.0f, +1.0f), XMFLOAT4(Colors::Blue) }),
			Vertex({ XMFLOAT3(-1.0f, +1.0f, +1.0f), XMFLOAT4(Colors::Yellow) }),
			Vertex({ XMFLOAT3(+1.0f, +1.0f, +1.0f), XMFLOAT4(Colors::Cyan) }),
			Vertex({ XMFLOAT3(+1.0f, -1.0f, +1.0f), XMFLOAT4(Colors::Magenta) })
		};

		std::array<std::uint16_t, 36> indices =
		{
			// front face
			0, 1, 2,
			0, 2, 3,

			// back face
			4, 6, 5,
			4, 7, 6,

			// left face
			4, 5, 1,
			4, 1, 0,

			// right face
			3, 2, 6,
			3, 6, 7,

			// top face
			1, 5, 6,
			1, 6, 2,

			// bottom face
			4, 0, 3,
			4, 3, 7
		};

		const UINT vbByteSize = (UINT)vertices.size() * sizeof(Vertex);
		const UINT ibByteSize = (UINT)indices.size() * sizeof(std::uint16_t);

		mBoxGeo = std::make_unique<MeshGeometry>();
		mBoxGeo->Name = "boxGeo";

		ThrowIfFailed(D3DCreateBlob(vbByteSize, &mBoxGeo->VertexBufferCPU));
		CopyMemory(mBoxGeo->VertexBufferCPU->GetBufferPointer(), vertices.data(), vbByteSize);

		ThrowIfFailed(D3DCreateBlob(ibByteSize, &mBoxGeo->IndexBufferCPU));
		CopyMemory(mBoxGeo->IndexBufferCPU->GetBufferPointer(), indices.data(), ibByteSize);

		
		mBoxGeo->VertexBufferGPU = d3dUtil::CreateDefaultBuffer(md3dDevice.Get(),
			mCommandList.Get(), vertices.data(), vbByteSize, mBoxGeo->VertexBufferUploader);

		mBoxGeo->IndexBufferGPU = d3dUtil::CreateDefaultBuffer(md3dDevice.Get(),
			mCommandList.Get(), indices.data(), ibByteSize, mBoxGeo->IndexBufferUploader);

		mBoxGeo->VertexByteStride = sizeof(Vertex);
		mBoxGeo->VertexBufferByteSize = vbByteSize;
		mBoxGeo->IndexFormat = DXGI_FORMAT_R16_UINT;
		mBoxGeo->IndexBufferByteSize = ibByteSize;

		SubmeshGeometry submesh;
		submesh.IndexCount = (UINT)indices.size();
		submesh.StartIndexLocation = 0;
		submesh.BaseVertexLocation = 0;

		mBoxGeo->DrawArgs["box"] = submesh;
	}
}