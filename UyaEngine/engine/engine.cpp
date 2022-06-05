#include "engine/engine.h"

using namespace uyatoy;


namespace uyatoy {
	UyaEngine::UyaEngine(HINSTANCE hInstance)
		: D3DApp(hInstance) {

	}

	UyaEngine::~UyaEngine() {

	}

	bool UyaEngine::Initialize() {
		if (!D3DApp::Initialize())
			return false;

		return true;
	}

	void UyaEngine::OnResize(){
		D3DApp::OnResize();
	}


	
}