#include "dx12Common/d3dApp.h"


namespace uyatoy {
	HRESULT EngineInit() {
		HRESULT hardwareResult = D3D12CreateDevice(
			nullptr,
			D3D_FEATURE_LEVEL_11_0,
			IID_PPV_ARGS(&md3dDevice));
			
	}
	

}