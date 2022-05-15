#include "dx12Common/d3dApp.h"

#include "render/include/render/render.h"

using namespace uyatoy;

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

}