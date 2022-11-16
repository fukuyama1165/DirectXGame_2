#include "bosstest.h"
#include <cassert>
#include <cstdlib>

//const Vector3 lerp(const Vector3& start, const Vector3& end, const float t);



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

	for (int i = 0; i < hand.size(); i++)
	{

		if (hand[i]->getisGetTargetPosFlag())
		{
			hand[i]->setTargetPos(player);
		}

	}

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
			bossStoneFall(player);

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
				hand[i]->setisPressEndFlag(false);
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

			if (pressCount < 3)
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
			}

			else
			{
				

				if (moveEndTimeCount == 0)
				{
					returnPos = worldTransform.matWorldGetPos();
				}

				if (moveEndTimeCount < maxMoveEndTime)
				{
					moveEndTimeCount++;

					worldTransform.translation_ = lerp(returnPos, { returnPos.x, 5, returnPos.z }, moveEndTimeCount / maxMoveEndTime);
					
					worldTransform.matWorldGeneration();
				}

				if (moveEndTimeCount == maxMoveEndTime)
				{
					pressCount = 0;
					setPressPos();
					moveEndTimeCount = 0;

					isBossPress = false;
					isPressStart = false;

					pressFirstStart = false;

					for (int i = 0; i < hand.size(); i++)
					{
						hand[i]->setisPressFlag(false);
					}
				}
			}
		}

	}

}

void bosstest::bossStoneFall(Vector3 player)
{

	if (isBossStoneFall)
	{
		if (waitTime == 0)
		{
			if (bossStoneFallCount == hand.size())
			{
				isBossStoneFall = false;
				bossStoneFallCount = 0;
				return;
			}

			for (int i = 0; i < hand.size(); i++)
			{
				if (hand[i]->getisAction() == false)
				{
					hand[i]->setisStoneFallFlag(true);
					waitTime = bossStoneFallWaitTime;
					bossStoneFallCount++;
					break;
				}
			}
		}

		if (waitTime > 0)
		{
			waitTime--;
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

void bosstest::setisBossStoneFall(bool flag)
{

	isBossStoneFall = flag;

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
	else if(hand[0]->getisPressEndd())
	{

		if (hand[0]->getisAction() == false)
		{
			hand[0]->setPos({ worldTransform.matWorldGetPos().x + 2.5f,worldTransform.matWorldGetPos().y + 2.5f,worldTransform.matWorldGetPos().z - 2.5f });
		}
		if (hand[1]->getisAction() == false)
		{
			hand[1]->setPos({ worldTransform.matWorldGetPos().x - 2.5f,worldTransform.matWorldGetPos().y + 2.5f,worldTransform.matWorldGetPos().z - 2.5f });
		}

		if (hand[2]->getisAction() == false)
		{
			hand[2]->setPos({ worldTransform.matWorldGetPos().x + 2.5f,worldTransform.matWorldGetPos().y - 2.5f,worldTransform.matWorldGetPos().z - 2.5f });
		}
		
		if (hand[3]->getisAction() == false)
		{
			hand[3]->setPos({ worldTransform.matWorldGetPos().x - 2.5f,worldTransform.matWorldGetPos().y - 2.5f,worldTransform.matWorldGetPos().z - 2.5f });
		}
		
		if (hand[4]->getisAction() == false)
		{
			hand[4]->setPos({ worldTransform.matWorldGetPos().x + 2.5f,worldTransform.matWorldGetPos().y + 2.5f,worldTransform.matWorldGetPos().z + 2.5f });
		}
		
		if (hand[5]->getisAction() == false)
		{
			hand[5]->setPos({ worldTransform.matWorldGetPos().x - 2.5f,worldTransform.matWorldGetPos().y + 2.5f,worldTransform.matWorldGetPos().z + 2.5f });
		}
		
		if (hand[6]->getisAction() == false)
		{
			hand[6]->setPos({ worldTransform.matWorldGetPos().x + 2.5f,worldTransform.matWorldGetPos().y - 2.5f,worldTransform.matWorldGetPos().z + 2.5f });
		}
		
		if (hand[7]->getisAction() == false)
		{
			hand[7]->setPos({ worldTransform.matWorldGetPos().x - 2.5f,worldTransform.matWorldGetPos().y - 2.5f,worldTransform.matWorldGetPos().z + 2.5f });
		}
	}
}

//const Vector3 lerp(const Vector3& start, const Vector3& end, const float t)
//{
//	return start +  t * (end - start);
//}

void bosstest::OnCollision(int damage)
{

	bossHP -= damage;

}


