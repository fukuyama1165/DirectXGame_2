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
	worldTransform.scale_ = { 3.0f,3.0f,3.0f };

	worldTransform.matWorldGeneration();

	for (int i = 0; i < 8; i++)
	{

		bossHand* newHand=new bossHand();
		newHand->init({ 3.0f,3.0f,3.0f }, {}, { sinf(i * 2) * 5,cosf(i * 2) * 5,200.0f }, model);

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

	if (!isBossBeam)
	{
		Vector3 front = nainavec3(player, worldTransform.matWorldGetPos());
		float fVec = atan2(front.x, front.z);
		worldTransform.rotation_.y = fVec;
	}
	else
	{
		worldTransform.rotation_.y = 0.0f;
	}

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
			
			setCubeDefaultPos();

			if (isHandMove == false)
			{
				setPressHandPos();
			}
			bossPress(player);
			bossStoneFall(player);
			bossBeam();
		}

		if (state == pillar)
		{

		}

		if (state == ophanim)
		{
			//hand[i]->setdefaultPos({ worldTransform.matWorldGetPos().x + sinf(i * 8) * 5,worldTransform.matWorldGetPos().y + cosf(i * 8) * 5,worldTransform.matWorldGetPos().z });
		}
		

	for (int i = 0; i < hand.size(); i++)
	{
		hand[i]->setRotate({ 0.0f,worldTransform.rotation_.y,0.0f });
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

				worldTransform.translation_ = lerp({ targetPos.x, pressPosY, targetPos.z }, { targetPos.x, 1.1f, targetPos.z }, fallTimeCount / maxfallTime);

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

					worldTransform.translation_ = lerp(returnPos, { returnPos.x, 7, returnPos.z }, moveEndTimeCount / maxMoveEndTime);
					
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

					setPressEnd();

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

void bosstest::bossBeam()
{

	if (isBossBeam)
	{
		if (waitTime > 0 and hand[0]->getisAction() == false)
		{
			waitTime--;
		}

		if (beamFirstStart == false and beamFirstMoveCount ==0)
		{
			
			beamStartPos = worldTransform.matWorldGetPos();
			beamFirstStart = true;

		}
		else if(beamFirstMoveCount < maxFirstBeamMoveTime)
		{
			
			beamFirstMoveCount++;
				
			worldTransform.translation_ = lerp(beamStartPos, { beamStartPos.x,beamStartPos.y,200 }, beamFirstMoveCount / maxFirstBeamMoveTime);

			worldTransform.matWorldGeneration();
	
		}

		if (bossBeamCount == 0 and hand[0]->getisAction() == false and beamFirstMoveCount == maxFirstBeamMoveTime)
		{
			hand[0]->setTargetPos({ worldTransform.matWorldGetPos().x, worldTransform.matWorldGetPos().y - 5, worldTransform.matWorldGetPos().z+200});
			hand[0]->setisBeamFlag(true);
			bossBeamCount++;
			waitTime = bossBeamWaitTime;
			return;
		}

		if (bossBeamCount == 1 and hand[0]->getisAction() == false and waitTime == 0)
		{
			hand[0]->setTargetPos({ worldTransform.matWorldGetPos().x, worldTransform.matWorldGetPos().y - 5, worldTransform.matWorldGetPos().z + 200 });
			hand[0]->setisBeamFlag(true);
			hand[1]->setTargetPos({ worldTransform.matWorldGetPos().x + 20, worldTransform.matWorldGetPos().y - 5,worldTransform.matWorldGetPos().z + 200 });
			hand[1]->setisBeamFlag(true);
			bossBeamCount++;
			return;
		}

		if (bossBeamCount == 2 and hand[0]->getisAction() == false and waitTime == 0)
		{

			if (beamEndMoveCount < maxEndBeamMoveTime)
			{
				beamEndMoveCount++;

				worldTransform.translation_ = lerp({ beamStartPos.x,beamStartPos.y,200 }, beamStartPos, beamEndMoveCount / maxEndBeamMoveTime);

				worldTransform.matWorldGeneration();
			}
			else
			{
				bossBeamCount = 0;
				beamFirstStart = false;
				isBossBeam = false;
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

void bosstest::setRotate(Vector3 rotate)
{
	worldTransform.rotation_ = rotate;
	worldTransform.matWorldGeneration();
}

void bosstest::setisAttackFlagL(bool flag,Vector3 player)
{
	for (int i = 0; i < hand.size(); i++)
	{
		hand[i]->setisAttackFlag(flag);
		hand[i]->setTargetPos(player);
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

void bosstest::setisBossBeam(bool flag)
{
	isBossBeam = flag;
}

void bosstest::setPressPos()
{
	Vector3 nannka;

	nannka = { 1,1,-1 };
	nannka = worldTransform.matWorld_.VectorMat(nannka, worldTransform.matWorld_);
	nannka.normalize();
	hand[0]->setTargetPos(worldTransform.matWorldGetPos() + (nannka * setbossCubePressDistance));

	nannka = { -1,1,-1 };
	nannka = worldTransform.matWorld_.VectorMat(nannka, worldTransform.matWorld_);
	nannka.normalize();
	hand[1]->setTargetPos(worldTransform.matWorldGetPos() + (nannka * setbossCubePressDistance));

	nannka = { 1,-1,-1 };
	nannka = worldTransform.matWorld_.VectorMat(nannka, worldTransform.matWorld_);
	nannka.normalize();
	hand[2]->setTargetPos(worldTransform.matWorldGetPos() + (nannka * setbossCubePressDistance));

	nannka = { -1,-1,-1 };
	nannka = worldTransform.matWorld_.VectorMat(nannka, worldTransform.matWorld_);
	nannka.normalize();
	hand[3]->setTargetPos(worldTransform.matWorldGetPos() + (nannka * setbossCubePressDistance));

	nannka = { 1,1,1 };
	nannka = worldTransform.matWorld_.VectorMat(nannka, worldTransform.matWorld_);
	nannka.normalize();
	hand[4]->setTargetPos(worldTransform.matWorldGetPos() + (nannka * setbossCubePressDistance));

	nannka = { -1,1,1 };
	nannka = worldTransform.matWorld_.VectorMat(nannka, worldTransform.matWorld_);
	nannka.normalize();
	hand[5]->setTargetPos(worldTransform.matWorldGetPos() + (nannka * setbossCubePressDistance));

	nannka = { 1,-1,1 };
	nannka = worldTransform.matWorld_.VectorMat(nannka, worldTransform.matWorld_);
	nannka.normalize();
	hand[6]->setTargetPos(worldTransform.matWorldGetPos() + (nannka * setbossCubePressDistance));

	nannka = { -1,-1,1 };
	nannka = worldTransform.matWorld_.VectorMat(nannka, worldTransform.matWorld_);
	nannka.normalize();
	hand[7]->setTargetPos(worldTransform.matWorldGetPos() + (nannka * setbossCubePressDistance));

	for (int i = 0; i < hand.size(); i++)
	{
		hand[i]->setisPressFlag(true);
	}

}

void bosstest::setPressHandPos()
{

	if (pressFirstStart)
	{
		Vector3 nannka;
		nannka = { 1,1,-1 };
		nannka = worldTransform.matWorld_.VectorMat(nannka, worldTransform.matWorld_);
		nannka.normalize();
		hand[0]->setPos(worldTransform.matWorldGetPos() + (nannka * setbossCubePressDistance));

		nannka = { -1,1,-1 };
		nannka = worldTransform.matWorld_.VectorMat(nannka, worldTransform.matWorld_);
		nannka.normalize();
		hand[1]->setPos(worldTransform.matWorldGetPos() + (nannka * setbossCubePressDistance));

		nannka = { 1,-1,-1 };
		nannka = worldTransform.matWorld_.VectorMat(nannka, worldTransform.matWorld_);
		nannka.normalize();
		hand[2]->setPos(worldTransform.matWorldGetPos() + (nannka * setbossCubePressDistance));

		nannka = { -1,-1,-1 };
		nannka = worldTransform.matWorld_.VectorMat(nannka, worldTransform.matWorld_);
		nannka.normalize();
		hand[3]->setPos(worldTransform.matWorldGetPos() + (nannka * setbossCubePressDistance));

		nannka = { 1,1,1 };
		nannka = worldTransform.matWorld_.VectorMat(nannka, worldTransform.matWorld_);
		nannka.normalize();
		hand[4]->setPos(worldTransform.matWorldGetPos() + (nannka * setbossCubePressDistance));

		nannka = { -1,1,1 };
		nannka = worldTransform.matWorld_.VectorMat(nannka, worldTransform.matWorld_);
		nannka.normalize();
		hand[5]->setPos(worldTransform.matWorldGetPos() + (nannka * setbossCubePressDistance));

		nannka = { 1,-1,1 };
		nannka = worldTransform.matWorld_.VectorMat(nannka, worldTransform.matWorld_);
		nannka.normalize();
		hand[6]->setPos(worldTransform.matWorldGetPos() + (nannka * setbossCubePressDistance));

		nannka = { -1,-1,1 };
		nannka = worldTransform.matWorld_.VectorMat(nannka, worldTransform.matWorld_);
		nannka.normalize();
		hand[7]->setPos(worldTransform.matWorldGetPos() + (nannka * setbossCubePressDistance));

	}
	else if(hand[0]->getisPressEndd())
	{

		Vector3 nannka;
		
		if (hand[0]->getisAction() == false)
		{
			nannka = { 1,1,-1 };
			nannka = worldTransform.matWorld_.VectorMat(nannka, worldTransform.matWorld_);
			nannka.normalize();
			hand[0]->setPos(worldTransform.matWorldGetPos() + (nannka * setbossCubeDistance));
		}
		if (hand[1]->getisAction() == false)
		{
			nannka = { -1,1,-1 };
			nannka = worldTransform.matWorld_.VectorMat(nannka, worldTransform.matWorld_);
			nannka.normalize();
			hand[1]->setPos(worldTransform.matWorldGetPos() + (nannka * setbossCubeDistance));
		}

		if (hand[2]->getisAction() == false)
		{

			nannka = { 1,-1,-1 };
			nannka = worldTransform.matWorld_.VectorMat(nannka, worldTransform.matWorld_);
			nannka.normalize();
			hand[2]->setPos(worldTransform.matWorldGetPos() + (nannka * setbossCubeDistance));
		}
		
		if (hand[3]->getisAction() == false)
		{
			nannka = { -1,-1,-1 };
			nannka = worldTransform.matWorld_.VectorMat(nannka, worldTransform.matWorld_);
			nannka.normalize();
			hand[3]->setPos(worldTransform.matWorldGetPos() + (nannka * setbossCubeDistance));
		}
		
		if (hand[4]->getisAction() == false)
		{
			nannka = { 1,1,1 };
			nannka = worldTransform.matWorld_.VectorMat(nannka, worldTransform.matWorld_);
			nannka.normalize();
			hand[4]->setPos(worldTransform.matWorldGetPos() + (nannka * setbossCubeDistance));
		}
		
		if (hand[5]->getisAction() == false)
		{
			nannka = { -1,1,1 };
			nannka = worldTransform.matWorld_.VectorMat(nannka, worldTransform.matWorld_);
			nannka.normalize();
			hand[5]->setPos(worldTransform.matWorldGetPos() + (nannka * setbossCubeDistance));
		}
		
		if (hand[6]->getisAction() == false)
		{
			nannka = { 1,-1,1 };
			nannka = worldTransform.matWorld_.VectorMat(nannka, worldTransform.matWorld_);
			nannka.normalize();
			hand[6]->setPos(worldTransform.matWorldGetPos() + (nannka * setbossCubeDistance));
		}
		
		if (hand[7]->getisAction() == false)
		{
			nannka = { -1,-1,1 };
			nannka = worldTransform.matWorld_.VectorMat(nannka, worldTransform.matWorld_);
			nannka.normalize();
			hand[7]->setPos(worldTransform.matWorldGetPos() + (nannka * setbossCubeDistance));
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

void  bosstest::setCubeDefaultPos()
{

	Vector3 nannka;

	if (hand[0]->getisAction() == false)
	{
		nannka = { 1,1,-1 };
		nannka = worldTransform.matWorld_.VectorMat(nannka, worldTransform.matWorld_);
		nannka.normalize();
		hand[0]->setdefaultPos(worldTransform.matWorldGetPos() + (nannka * setbossCubeDistance));
	}
	if (hand[1]->getisAction() == false)
	{
		nannka = { -1,1,-1 };
		nannka = worldTransform.matWorld_.VectorMat(nannka, worldTransform.matWorld_);
		nannka.normalize();
		hand[1]->setdefaultPos(worldTransform.matWorldGetPos() + (nannka * setbossCubeDistance));
	}

	if (hand[2]->getisAction() == false)
	{

		nannka = { 1,-1,-1 };
		nannka = worldTransform.matWorld_.VectorMat(nannka, worldTransform.matWorld_);
		nannka.normalize();
		hand[2]->setdefaultPos(worldTransform.matWorldGetPos() + (nannka * setbossCubeDistance));
	}

	if (hand[3]->getisAction() == false)
	{
		nannka = { -1,-1,-1 };
		nannka = worldTransform.matWorld_.VectorMat(nannka, worldTransform.matWorld_);
		nannka.normalize();
		hand[3]->setdefaultPos(worldTransform.matWorldGetPos() + (nannka * setbossCubeDistance));
	}

	if (hand[4]->getisAction() == false)
	{
		nannka = { 1,1,1 };
		nannka = worldTransform.matWorld_.VectorMat(nannka, worldTransform.matWorld_);
		nannka.normalize();
		hand[4]->setdefaultPos(worldTransform.matWorldGetPos() + (nannka * setbossCubeDistance));
	}

	if (hand[5]->getisAction() == false)
	{
		nannka = { -1,1,1 };
		nannka = worldTransform.matWorld_.VectorMat(nannka, worldTransform.matWorld_);
		nannka.normalize();
		hand[5]->setdefaultPos(worldTransform.matWorldGetPos() + (nannka * setbossCubeDistance));
	}

	if (hand[6]->getisAction() == false)
	{
		nannka = { 1,-1,1 };
		nannka = worldTransform.matWorld_.VectorMat(nannka, worldTransform.matWorld_);
		nannka.normalize();
		hand[6]->setdefaultPos(worldTransform.matWorldGetPos() + (nannka * setbossCubeDistance));
	}

	if (hand[7]->getisAction() == false)
	{
		nannka = { -1,-1,1 };
		nannka = worldTransform.matWorld_.VectorMat(nannka, worldTransform.matWorld_);
		nannka.normalize();
		hand[7]->setdefaultPos(worldTransform.matWorldGetPos() + (nannka * setbossCubeDistance));
	}

	for (int i = 0; i < hand.size(); i++)
	{

		if (hand[i]->getisAction() == false)
		{
			hand[i]->setScale({ 3.0f,3.0f,3.0f });
		}

	}

}


void bosstest::setPressEnd()
{

	
	Vector3 nannka;

		nannka = { 1,1,-1 };
		nannka = worldTransform.matWorld_.VectorMat(nannka, worldTransform.matWorld_);
		nannka.normalize();
		hand[0]->setdefaultPos(worldTransform.matWorldGetPos() + (nannka * setbossCubeDistance));
	
		nannka = { -1,1,-1 };
		nannka = worldTransform.matWorld_.VectorMat(nannka, worldTransform.matWorld_);
		nannka.normalize();
		hand[1]->setdefaultPos(worldTransform.matWorldGetPos() + (nannka * setbossCubeDistance));
	
		nannka = { 1,-1,-1 };
		nannka = worldTransform.matWorld_.VectorMat(nannka, worldTransform.matWorld_);
		nannka.normalize();
		hand[2]->setdefaultPos(worldTransform.matWorldGetPos() + (nannka * setbossCubeDistance));

		nannka = { -1,-1,-1 };
		nannka = worldTransform.matWorld_.VectorMat(nannka, worldTransform.matWorld_);
		nannka.normalize();
		hand[3]->setdefaultPos(worldTransform.matWorldGetPos() + (nannka * setbossCubeDistance));
	
		nannka = { 1,1,1 };
		nannka = worldTransform.matWorld_.VectorMat(nannka, worldTransform.matWorld_);
		nannka.normalize();
		hand[4]->setdefaultPos(worldTransform.matWorldGetPos() + (nannka * setbossCubeDistance));
	
		nannka = { -1,1,1 };
		nannka = worldTransform.matWorld_.VectorMat(nannka, worldTransform.matWorld_);
		nannka.normalize();
		hand[5]->setdefaultPos(worldTransform.matWorldGetPos() + (nannka * setbossCubeDistance));
	
		nannka = { 1,-1,1 };
		nannka = worldTransform.matWorld_.VectorMat(nannka, worldTransform.matWorld_);
		nannka.normalize();
		hand[6]->setdefaultPos(worldTransform.matWorldGetPos() + (nannka * setbossCubeDistance));
	
		nannka = { -1,-1,1 };
		nannka = worldTransform.matWorld_.VectorMat(nannka, worldTransform.matWorld_);
		nannka.normalize();
		hand[7]->setdefaultPos(worldTransform.matWorldGetPos() + (nannka * setbossCubeDistance));

}