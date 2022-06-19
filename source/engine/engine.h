#include "dx12Common/d3dApp.h"

using namespace DirectX;


namespace Uya {

	class UyaEngine : public D3DApp {
	public:
		UyaEngine(HINSTANCE hInstance);
		~UyaEngine();

		virtual bool Initialize()override;

	private:
		virtual void OnResize()override;

	protected:
		std::wstring mMainWndCaption = L"Uya d3d12 App";
		

	};


}//namespace Uya