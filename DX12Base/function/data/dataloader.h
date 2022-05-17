#include "dx12Common/d3dApp.h"

using namespace DirectX;

namespace uyatoy {

	struct Vertex {
		XMFLOAT3 Pos;
		XMFLOAT4 Color;
	};

	D3D12_INPUT_ELEMENT_DESC desc1[] = {
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
		{"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0}
	};

	struct Vertext2 {
		XMFLOAT3 Pos;
		XMFLOAT3 Normal;
		XMFLOAT2 Tex0;
		XMFLOAT2 Tex1;
	};

	D3D12_INPUT_ELEMENT_DESC desc2[] = {
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
		{"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
		{"TEXCOORD", 1, DXGI_FORMAT_R32G32_FLOAT, 0, 32,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0}
	};

	class DataLoader {
	public:
		DataLoader(Microsoft::WRL::ComPtr<ID3D12Device> d3dDevice,
			Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> CommandList);
		~DataLoader();

		std::unique_ptr<MeshGeometry> BuildSphereGeometry(XMFLOAT3 center, float rad);
		std::unique_ptr<MeshGeometry> BuildBoxGeometry();
	private:
		std::unique_ptr<MeshGeometry> mBoxGeo = nullptr;
		Microsoft::WRL::ComPtr<ID3D12Device> md3dDevice = nullptr;
		Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> mCommandList = nullptr;

	};

}