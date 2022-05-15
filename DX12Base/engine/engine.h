#include "dx12Common/d3dApp.h"

using namespace DirectX;
using namespace uyatoy;

int WINAPI WinMain(
	HINSTANCE hInstance, HINSTANCE prevInstance,
	PSTR cmdLine, int showCmd)
{
	// Enable run-time memory check for debug builds.
#if defined(DEBUG) | defined(_DEBUG)
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif
	try
	{
		UyaEngine theApp(hInstance);
		if (!theApp.Initialize())
			return 0;

		return theApp.Run();
	}
	catch (DxException& e)
	{
		MessageBox(nullptr, e.ToString().c_str(), L"HR Failed", MB_OK);
		return 0;
	}
}

namespace uyatoy {

	class UyaEngine : public D3DApp {
	public:
		UyaEngine(HINSTANCE hInstance);
		~UyaEngine();

		virtual bool Initialize()override;

	private:
		virtual void OnResize()override;
		virtual void Update(const GameTimer& gt)override;
		virtual void Draw(const GameTimer& gt)override;

	};

	bool UyaEngine::Initialize() {
		if (!D3DApp::Initialize())
			return false;

		return true;
	}

	void UyaEngine::OnResize()
	{
		D3DApp::OnResize();
	}

	HRESULT EngineInit() {
		
			
	}
	

}