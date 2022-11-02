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
	// �{���̓O���[�o���ɂ��Ȃ������ǂ�
	const float MIN_FREAM_TIME = 1.0f / 80;
	float frameTime = 0;
	LARGE_INTEGER timeStart;
	LARGE_INTEGER timeEnd;
	LARGE_INTEGER timeFreq;
	// fps���擾����Ȃ�0�ŏ��������Ȃ��ƃS�~���������ă}�C�i�X����n�܂����肷��(�����m��Ȃ�)
	float fps = 0;

private:

	DebugText* debugText_ = nullptr;
};

