#include "bosstest.h"
#include <cassert>
#include <cstdlib>

//const Vector3 lerp(const Vector3& start, const Vector3& end, const float t);

//単純に==
bool vector3Issame(Vector3 a, Vector3 b);

bosstest::bosstest()
{

}
bosstest::~bosstest()
{
	for (int i = 0; i < hand.size(); i++)
	{
		delete hand[i];
	}
}

void bosstest::Initialize(Model* model, const Vector3& position)
{

	assert(model);
	model_ = model;

	debugText_ = DebugText::GetInstance();

	//ワールドトランスフォームの初期化
	worldTransform.Initialize();

	//引数で受け取った初期座標をセット
	worldTransform.translation_ = position;
	worldTransform.scale_ = { 3,3,3 };

	worldTransform.matWorldGeneration();

	for (int i = 0; i < 8; i++)
	{

		bossHand* newHand=new bossHand();
		newHand->init({ 1.0f,1.0f,1.0f }, {}, { sinf(i * 2) * 5,cosf(i * 2) * 5,200 }, model);

		hand.push_back(newHand);
	}

	setPressHandPos();
	
	
	/*Hand[1].init({ 1.0f,1.0f,1.0f }, {}, { -4.0f,-0.5f,200 }, model);
	Hand[2].init({ 1.0f,1.0f,1.0f }, {}, { 4.0f,0,200 }, model);
	Hand[3].init({1.0f,1.0f,1.0f}, {}, {4.0f,0,200}, model);
	Hand[4].init({1.0f,1.0f,1.0f}, {}, {4.0f,0,200}, model);
	Hand[5].init({1.0f,1.0f,1.0f}, {}, {4.0f,0,200}, model);
	Hand[6].init({1.0f,1.0f,1.0f}, {}, {4.0f,0,200}, model);
	Hand[7].init({1.0f,1.0f,1.0f}, {}, {4.0f,0,200}, model);*/

	


	/*rightHand.scale_ = { 1.0f,1.0f,1.0f };
	rightHand.translation_ = { -4.0f,-0.5f,200 };

	rightHand.matWorldGeneration();*/


}

void bosstest::Update(Vector3 player)
{

	

	worldTransform.matWorldGeneration();

	if (state == Cube)
	{
		hand[0]->setdefaultPos({ worldTransform.matWorldGetPos().x + 2.5f,worldTransform.matWorldGetPos().y + 2.5f,worldTransform.matWorldGetPos().z - 2.5f });
		hand[1]->setdefaultPos({ worldTransform.matWorldGetPos().x - 2.5f,worldTransform.matWorldGetPos().y + 2.5f,worldTransform.matWorldGetPos().z - 2.5f });
		hand[2]->setdefaultPos({ worldTransform.matWorldGetPos().x + 2.5f,worldTransform.matWorldGetPos().y - 2.5f,worldTransform.matWorldGetPos().z - 2.5f });
		hand[3]->setdefaultPos({ worldTransform.matWorldGetPos().x - 2.5f,worldTransform.matWorldGetPos().y - 2.5f,worldTransform.matWorldGetPos().z - 2.5f });
		hand[4]->setdefaultPos({ worldTransform.matWorldGetPos().x + 2.5f,worldTransform.matWorldGetPos().y + 2.5f,worldTransform.matWorldGetPos().z + 2.5f });
		hand[5]->setdefaultPos({ worldTransform.matWorldGetPos().x - 2.5f,worldTransform.matWorldGetPos().y + 2.5f,worldTransform.matWorldGetPos().z + 2.5f });
		hand[6]->setdefaultPos({ worldTransform.matWorldGetPos().x + 2.5f,worldTransform.matWorldGetPos().y - 2.5f,worldTransform.matWorldGetPos().z + 2.5f });
		hand[7]->setdefaultPos({ worldTransform.matWorldGetPos().x - 2.5f,worldTransform.matWorldGetPos().y - 2.5f,worldTransform.matWorldGetPos().z + 2.5f });
	}

	for (int i = 0; i < hand.size(); i++)
	{

		if (state == Cube)
		{
			hand[i]->setScale({ 2,2,2 });
			if (isHandMove == false)
			{
				setPressHandPos();
			}
			bossPress(player);

		}

		if (state == pillar)
		{

		}

		if (state == ophanim)
		{
			hand[i]->setdefaultPos({ worldTransform.matWorldGetPos().x + sinf(i * 8) * 5,worldTransform.matWorldGetPos().y + cosf(i * 8) * 5,worldTransform.matWorldGetPos().z });
		}
		

	}
	for (int i = 0; i < hand.size(); i++)
	{
		hand[i]->update(worldTransform);
	}
	
}

void bosstest::Draw(const ViewProjection& viewProjection)
{
	
	for (int i = 0; i < hand.size(); i++)
	{
		hand[i]->draw(viewProjection);
		debugText_->SetPos(0, i * 20 + 50);
		debugText_->Printf("%d", hand[i]->getisReturnHand());
	}
	model_->Draw(worldTransform, viewProjection);
	
}

void bosstest::bossPress(Vector3 player)
{

	if (isBossPress)
	{
		if (pressFirstStart == false)
		{
			setPressPos();

			for (int i = 0; i < hand.size(); i++)
			{
				hand[i]->setisPressFlag(true);
			}

			pressFirstStart = true;

			isPressReturn = true;
		}

		if (isPressStart)
		{

			if (timeCount == 0)
			{
				targetPos = player;
			}

			if (timeCount < maxTime)
			{
				timeCount++;
			}

			if (timeCount != maxTime)
			{

				worldTransform.translation_ = lerp(returnPos, { targetPos.x, pressPosY, targetPos.z }, timeCount / maxTime);

				worldTransform.matWorldGeneration();

			}

			if (timeCount == maxTime)
			{

				isPressStart = false;
				isPressFall = true;
				timeCount = 0;

			}

		}

		if (isPressFall)
		{

			if (waitTime < fallWaitTime)
			{

				worldTransform.translation_ = { cosf(waitTime) + targetPos.x, pressPosY, targetPos.z };

				worldTransform.matWorldGeneration();

			}

			if (fallTimeCount < maxfallTime and waitTime >= fallWaitTime)
			{
				fallTimeCount++;
			}

			if (fallTimeCount != maxfallTime and waitTime >= fallWaitTime)
			{

				worldTransform.translation_ = lerp({ targetPos.x, pressPosY, targetPos.z }, { targetPos.x, 0, targetPos.z }, fallTimeCount / maxfallTime);

				worldTransform.matWorldGeneration();

			}

			if (waitTime < fallWaitTime)
			{
				waitTime++;
			}

			if (fallTimeCount == maxfallTime)
			{

				isPressFall = false;
				isPressReturn = true;
				fallTimeCount = 0;
				waitTime = 0;

			}

		}

		if (isPressReturn)
		{

			if (waitTime >= returnWaitTime)
			{

				returnPos = worldTransform.matWorldGetPos();

			}

			if (returnTimeCount < maxReturnTime and waitTime >= returnWaitTime)
			{
				returnTimeCount++;
			}

			if (returnTimeCount != maxReturnTime and waitTime >= returnWaitTime)
			{

				worldTransform.translation_ = lerp(returnPos, { returnPos.x, pressPosY, returnPos.z }, returnTimeCount / maxReturnTime);

				worldTransform.matWorldGeneration();

			}

			if (waitTime < returnWaitTime)
			{
				waitTime++;
			}


			if (returnTimeCount == maxReturnTime)
			{

				isPressReturn = false;

				isPressStart = true;

				waitTime = 0;
				returnTimeCount = 0;

				pressCount++;

			}

			if (pressCount == 3)
			{
				isBossPress = false;
				isPressStart = false;

				pressFirstStart = false;

				pressCount = 0;
				setPressPos();

				for (int i = 0; i < hand.size(); i++)
				{
					hand[i]->setisPressFlag(false);
				}
			}
		}

	}

}

Vector3 bosstest::GetWorldPosition()
{
	return worldTransform.matWorldGetPos();
}

void bosstest::setPos(Vector3 pos)
{

	worldTransform.translation_ = pos;
	worldTransform.matWorldGeneration();

}

void bosstest::setisAttackFlagL(bool flag)
{
	for (int i = 0; i < hand.size(); i++)
	{
		hand[i]->setisAttackFlag(flag);
	}
}

void bosstest::playerAttackReturnL()
{
	for (int i = 0; i < hand.size(); i++)
	{
		hand[i]->playerAttackReturn();
	}
}

void bosstest::setisBossPress(bool flag)
{
	isBossPress = flag;
}

void bosstest::setPressPos()
{

	hand[0]->setTargetPos({ worldTransform.matWorldGetPos().x + 2.0f,worldTransform.matWorldGetPos().y + 2.0f,worldTransform.matWorldGetPos().z - 2.0f });
	hand[1]->setTargetPos({ worldTransform.matWorldGetPos().x - 2.0f,worldTransform.matWorldGetPos().y + 2.0f,worldTransform.matWorldGetPos().z - 2.0f });
	hand[2]->setTargetPos({ worldTransform.matWorldGetPos().x + 2.0f,worldTransform.matWorldGetPos().y - 2.0f,worldTransform.matWorldGetPos().z - 2.0f });
	hand[3]->setTargetPos({ worldTransform.matWorldGetPos().x - 2.0f,worldTransform.matWorldGetPos().y - 2.0f,worldTransform.matWorldGetPos().z - 2.0f });
	hand[4]->setTargetPos({ worldTransform.matWorldGetPos().x + 2.0f,worldTransform.matWorldGetPos().y + 2.0f,worldTransform.matWorldGetPos().z + 2.0f });
	hand[5]->setTargetPos({ worldTransform.matWorldGetPos().x - 2.0f,worldTransform.matWorldGetPos().y + 2.0f,worldTransform.matWorldGetPos().z + 2.0f });
	hand[6]->setTargetPos({ worldTransform.matWorldGetPos().x + 2.0f,worldTransform.matWorldGetPos().y - 2.0f,worldTransform.matWorldGetPos().z + 2.0f });
	hand[7]->setTargetPos({ worldTransform.matWorldGetPos().x - 2.0f,worldTransform.matWorldGetPos().y - 2.0f,worldTransform.matWorldGetPos().z + 2.0f });

	for (int i = 0; i < hand.size(); i++)
	{
		hand[i]->setisPressFlag(true);
	}

}

void bosstest::setPressHandPos()
{

	if (pressFirstStart)
	{
	
		hand[0]->setPos({ worldTransform.matWorldGetPos().x + 2.0f,worldTransform.matWorldGetPos().y + 2.0f,worldTransform.matWorldGetPos().z - 2.0f });
		hand[1]->setPos({ worldTransform.matWorldGetPos().x - 2.0f,worldTransform.matWorldGetPos().y + 2.0f,worldTransform.matWorldGetPos().z - 2.0f });
		hand[2]->setPos({ worldTransform.matWorldGetPos().x + 2.0f,worldTransform.matWorldGetPos().y - 2.0f,worldTransform.matWorldGetPos().z - 2.0f });
		hand[3]->setPos({ worldTransform.matWorldGetPos().x - 2.0f,worldTransform.matWorldGetPos().y - 2.0f,worldTransform.matWorldGetPos().z - 2.0f });
		hand[4]->setPos({ worldTransform.matWorldGetPos().x + 2.0f,worldTransform.matWorldGetPos().y + 2.0f,worldTransform.matWorldGetPos().z + 2.0f });
		hand[5]->setPos({ worldTransform.matWorldGetPos().x - 2.0f,worldTransform.matWorldGetPos().y + 2.0f,worldTransform.matWorldGetPos().z + 2.0f });
		hand[6]->setPos({ worldTransform.matWorldGetPos().x + 2.0f,worldTransform.matWorldGetPos().y - 2.0f,worldTransform.matWorldGetPos().z + 2.0f });
		hand[7]->setPos({ worldTransform.matWorldGetPos().x - 2.0f,worldTransform.matWorldGetPos().y - 2.0f,worldTransform.matWorldGetPos().z + 2.0f });

	}
	else
	{

		hand[0]->setPos({ worldTransform.matWorldGetPos().x + 2.5f,worldTransform.matWorldGetPos().y + 2.5f,worldTransform.matWorldGetPos().z - 2.5f });
		hand[1]->setPos({ worldTransform.matWorldGetPos().x - 2.5f,worldTransform.matWorldGetPos().y + 2.5f,worldTransform.matWorldGetPos().z - 2.5f });
		hand[2]->setPos({ worldTransform.matWorldGetPos().x + 2.5f,worldTransform.matWorldGetPos().y - 2.5f,worldTransform.matWorldGetPos().z - 2.5f });
		hand[3]->setPos({ worldTransform.matWorldGetPos().x - 2.5f,worldTransform.matWorldGetPos().y - 2.5f,worldTransform.matWorldGetPos().z - 2.5f });
		hand[4]->setPos({ worldTransform.matWorldGetPos().x + 2.5f,worldTransform.matWorldGetPos().y + 2.5f,worldTransform.matWorldGetPos().z + 2.5f });
		hand[5]->setPos({ worldTransform.matWorldGetPos().x - 2.5f,worldTransform.matWorldGetPos().y + 2.5f,worldTransform.matWorldGetPos().z + 2.5f });
		hand[6]->setPos({ worldTransform.matWorldGetPos().x + 2.5f,worldTransform.matWorldGetPos().y - 2.5f,worldTransform.matWorldGetPos().z + 2.5f });
		hand[7]->setPos({ worldTransform.matWorldGetPos().x - 2.5f,worldTransform.matWorldGetPos().y - 2.5f,worldTransform.matWorldGetPos().z + 2.5f });

	}
}

//const Vector3 lerp(const Vector3& start, const Vector3& end, const float t)
//{
//	return start +  t * (end - start);
//}



bool vector3Issame(Vector3 a, Vector3 b)
{

	if (a.x == b.x and a.y == b.y and a.z == b.z)
	{
		return true;
	}

	return false;

}
