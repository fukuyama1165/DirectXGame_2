#pragma once
#include <Windows.h>
#include <tchar.h>
#include <sstream>
#include <mmsystem.h>
#pragma comment(lib,"winmm.lib")
#include "DebugText.h"

class FPS
{
public:
	FPS();
	~FPS();

	int Init();

	void Ran();

	void FPSDraw();

public:
	// 本当はグローバルにしない方が良い
	const float MIN_FREAM_TIME = 1.0f / 80;
	float frameTime = 0;
	LARGE_INTEGER timeStart;
	LARGE_INTEGER timeEnd;
	LARGE_INTEGER timeFreq;
	// fpsを取得するなら0で初期化しないとゴミが混ざってマイナスから始まったりする(かも知れない)
	float fps = 0;

private:

	DebugText* debugText_ = nullptr;
};

