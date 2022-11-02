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

	//数値を画像で表示する関数(一番左の桁の左上のXY)※仕様的に使えない
	void graphNumbersDisplay(float X, float Y, int number);

	//桁を決め全てに数字を画像で表示する関数(数字がない場合は０を出力)(１０桁までしか描画できない)
	void graphNumberDisplayScore(float X, float Y, int number, const int rank);

	//2dレティクル用スプライト
	std::unique_ptr<Sprite> sprite;

private:

	int numberOutput(int number);
	int exponentiation10(int a);

private:

	

	uint32_t textureHandle[10]={};
	
	Sprite* rankNum[MAXRANK] = {};

};

