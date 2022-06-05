#include "dx12Common/d3dApp.h"

using namespace DirectX;


namespace uyatoy {

	class UyaEngine : public D3DApp {
	public:
		UyaEngine(HINSTANCE hInstance);
		~UyaEngine();

		virtual bool Initialize()override;

	private:
		virtual void OnResize()override;
		

	};

	
	

}