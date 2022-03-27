#include <windows.h>
#include <iostream>

using namespace std;

__int64 mCurrTime;
__int64 mPrevTime;
double mSecondsPerCount;
double mDeltaTime;

int main()
{
	__int64 countsPerSecFreq;
	QueryPerformanceFrequency((LARGE_INTEGER*)&countsPerSecFreq);
	mSecondsPerCount = 1.0 / (double)countsPerSecFreq;
	Microsoft::WRL::ComPtr<ID3D12CommandQueue>
		mCommandQueue;
	D3D12_COMMAND_QUEUE_DESC queueDesc = {};
	queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
	queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
	ThrowIfFailed(md3dDevice->CreateCommandQueue(
		&queueDesc, IID_PPV_ARGS(&mCommandQueue)));
	while (true)
	{
		__int64 currTime;
		QueryPerformanceCounter((LARGE_INTEGER*)&currTime);
		mCurrTime = currTime;

		Sleep(100);

		// Time difference between this frame and the previous.
		mDeltaTime = (mCurrTime - mPrevTime) * mSecondsPerCount;

		// Prepare for next frame.
		mPrevTime = mCurrTime;

		cout << (double)mCurrTime / mSecondsPerCount << endl;
	}
	return 0;
}