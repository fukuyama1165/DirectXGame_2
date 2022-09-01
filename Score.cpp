#include "Score.h"
float Score::Scorenum;
float Score::HiScorenum;

Score::Score()
{
}

Score::~Score()
{
	for (int i = 0; i < MAXRANK; i++)
	{
		delete rankNum[i];
	}
}

void Score::init(uint32_t textureHandle0, uint32_t textureHandle1, uint32_t textureHandle2, uint32_t textureHandle3, uint32_t textureHandle4, uint32_t textureHandle5, uint32_t textureHandle6, uint32_t textureHandle7, uint32_t textureHandle8, uint32_t textureHandle9)
{

	textureHandle[0] = textureHandle0;
	textureHandle[1] = textureHandle1;
	textureHandle[2] = textureHandle2;
	textureHandle[3] = textureHandle3;
	textureHandle[4] = textureHandle4;
	textureHandle[5] = textureHandle5;
	textureHandle[6] = textureHandle6;
	textureHandle[7] = textureHandle7;
	textureHandle[8] = textureHandle8;
	textureHandle[9] = textureHandle9;

	sprite.reset(Sprite::Create(textureHandle[0], { 640,360 }, { 1,1,1,1 }, { 0.5f,0.5f }));

	for (int i = 0; i < MAXRANK; i++)
	{
		rankNum[i] = Sprite::Create(textureHandle[0], {640,360}, {1,1,1,1}, {0.5f,0.5f});
	}

}

//数値を画像で表示する関数(一番左の桁の左上のXY)
void Score::graphNumbersDisplay(float X, float Y, int number)
{
	int rank = 1;
	int rankNumber = number;
	int outputNumber = 0;
	while (rankNumber / 10 != 0)
	{
		rankNumber = rankNumber / 10;
		rank++;
	}
	while (rank > 0)
	{
		outputNumber = numberOutput(number);
		number = number / 10;
		switch (outputNumber)
		{
		case 0:
			sprite->SetTextureHandle(textureHandle[0]);
			break;
		case 1:
			sprite->SetTextureHandle(textureHandle[1]);
			break;
		case 2:
			sprite->SetTextureHandle(textureHandle[2]);
			break;
		case 3:
			sprite->SetTextureHandle(textureHandle[3]);
			break;
		case 4:
			sprite->SetTextureHandle(textureHandle[4]);
			break;
		case 5:
			sprite->SetTextureHandle(textureHandle[5]);
			break;
		case 6:
			sprite->SetTextureHandle(textureHandle[6]);
			break;
		case 7:
			sprite->SetTextureHandle(textureHandle[7]);
			break;
		case 8:
			sprite->SetTextureHandle(textureHandle[8]);
			break;
		case 9:
			sprite->SetTextureHandle(textureHandle[9]);
			break;
		default:
			break;
		}

		sprite->SetPosition(Vector2(X + 64 * rank, Y));
		sprite->Draw();
		rank--;
	}

}

//もらった数値の一桁目を取り出す関数
int Score::numberOutput(int number)
{
	return number % 10;
}

//桁を決め全てに数字を画像で表示する関数(数字がない場合は０を出力)
void Score::graphNumberDisplayScore(float X, float Y, int number,const int rank)
{
	float posX = X + 16;
	float posY = Y + 16;

	int outputNumber = 0;
	
	for (int i = rank-1; i > -1; i--)
	{
		outputNumber = number / exponentiation10(i);
		if (outputNumber > 9)
		{
			outputNumber = outputNumber % 10;
		}
		switch (outputNumber)
		{
		case 0:
			rankNum[i]->SetTextureHandle(textureHandle[0]);
			break;
		case 1:
			rankNum[i]->SetTextureHandle(textureHandle[1]);
			break;
		case 2:
			rankNum[i]->SetTextureHandle(textureHandle[2]);
			break;
		case 3:
			rankNum[i]->SetTextureHandle(textureHandle[3]);
			break;
		case 4:
			rankNum[i]->SetTextureHandle(textureHandle[4]);
			break;
		case 5:
			rankNum[i]->SetTextureHandle(textureHandle[5]);
			break;
		case 6:
			rankNum[i]->SetTextureHandle(textureHandle[6]);
			break;
		case 7:
			rankNum[i]->SetTextureHandle(textureHandle[7]);
			break;
		case 8:
			rankNum[i]->SetTextureHandle(textureHandle[8]);
			break;
		case 9:
			rankNum[i]->SetTextureHandle(textureHandle[9]);
			break;
		default:

			break;
		}

		rankNum[i]->SetPosition(Vector2(posX + 32 * ((rank - i)-1), posY));
		rankNum[i]->Draw();
		
	}
}

//累乗の関数
int Score::exponentiation10(int a)
{
	int ans = 0;
	if (a == 0)
	{
		return 1;
	}

	for (int i = 0; i < a; i++)
	{
		if (ans == 0)
		{
			ans = 10;
		}
		else
		{
			ans = ans * 10;
		}
	}

	return ans;
}