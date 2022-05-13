
#include "dataloader.h"
#include "Common/GameTimer.h"
#include "Common/d3dApp.h"
#include "Common/MathHelper.h"
#include "Common/UploadBuffer.h"

using namespace std;
using namespace DirectX::PackedVector;
using Microsoft::WRL::ComPtr;
using namespace cimg_library;


namespace DirectX {
	
	template<class T>
	struct Color3
	{
		T R;
		T G;
		T B;
	};

	template<class T>
	struct Color4
	{
		T R;
		T G;
		T B;
		T A;
	};

	struct TextureHead {
		uint32_t size;
		uint32_t height;
		uint32_t width;
		uint32_t depth; //only if DDS_HEADER_FLAGS_VOLUME is set in flags
		uint32_t mipMapCount;
		uint32_t rgbaChanelNums;
	};

	

	HRESULT CreateBMPTextureFromfile(
		_In_ ID3D12Device* device,
		_In_ ID3D12GraphicsCommandList* cmdList,
		_In_z_ const char* szFileName,
		_Out_ Microsoft::WRL::ComPtr<ID3D12Resource>& texture,
		_Out_ Microsoft::WRL::ComPtr<ID3D12Resource>& textureUploadHeap
	) {
		if (!device || !szFileName) return E_INVALIDARG;
		
		TextureHead header;

		CImg<uint8_t> ReadTexture(szFileName);
		header.height = ReadTexture.height();
		header.width = ReadTexture.width();
		header.depth = ReadTexture.depth();
		header.mipMapCount = 0;
		header.rgbaChanelNums = ReadTexture.spectrum();
		header.size = ReadTexture.size();

		vector<Color3<uint8_t>> ReadTextureBulkData;
		vector<Color4<uint8_t>> InitTextureBulkData;

		cimg_forXY(ReadTexture, x, y) {
			Color3<uint8_t> NewColor;
			NewColor.R = ReadTexture(x, y, 0);
			NewColor.G = ReadTexture(x, y, 1);
			NewColor.B = ReadTexture(x, y, 2);
			ReadTextureBulkData.push_back(std::move(NewColor));
			//reading to readTexBulk
		}

		for (int i = 0; i < ReadTextureBulkData.size(); i++)
		{
			Color4<uint8_t> NewColor;
			NewColor.R = ReadTextureBulkData[i].R;
			NewColor.G = ReadTextureBulkData[i].G;
			NewColor.B = ReadTextureBulkData[i].B;
			NewColor.A = 255;
			InitTextureBulkData.push_back(NewColor);
			//reading to initTexBulk
		}

		int pixel_data_size = InitTextureBulkData.size() * sizeof(Color4<uint8_t>);

		D3D12_RESOURCE_DESC texDesc;
		memset(&texDesc, 0, sizeof(D3D12_RESOURCE_DESC));
		texDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
		texDesc.Alignment = 0;
		texDesc.Width = (uint32_t)header.width;
		texDesc.Height = (uint32_t)header.height;
		texDesc.DepthOrArraySize = 1;
		texDesc.MipLevels = 1;
		texDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		texDesc.SampleDesc.Count = 1;
		texDesc.SampleDesc.Quality = 0;
		texDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
		texDesc.Flags = D3D12_RESOURCE_FLAG_NONE;




	}
}
