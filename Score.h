#include "DebugText.h"
#include <memory>

const int MAXRANK = 10;

class Score
{
public:
	Score();
	~Score();

	static float Scorenum;
	static float HiScorenum;

	void init(uint32_t textureHandle0, uint32_t textureHandle1, uint32_t textureHandle2, uint32_t textureHandle3, uint32_t textureHandle4, uint32_t textureHandle5, uint32_t textureHandle6, uint32_t textureHandle7, uint32_t textureHandle8, uint32_t textureHandle9);

	//���l���摜�ŕ\������֐�(��ԍ��̌��̍����XY)���d�l�I�Ɏg���Ȃ�
	void graphNumbersDisplay(float X, float Y, int number);

	//�������ߑS�Ăɐ������摜�ŕ\������֐�(�������Ȃ��ꍇ�͂O���o��)(�P�O���܂ł����`��ł��Ȃ�)
	void graphNumberDisplayScore(float X, float Y, int number, const int rank);

	//2d���e�B�N���p�X�v���C�g
	std::unique_ptr<Sprite> sprite;

private:

	int numberOutput(int number);
	int exponentiation10(int a);

private:

	

	uint32_t textureHandle[10]={};
	
	Sprite* rankNum[MAXRANK] = {};

};

