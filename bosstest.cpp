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

void bosstest::Initialize(Model* model, Model* homdmodel, const Vector3& position)
{

	assert(model);
	assert(homdmodel);
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
		newHand->init({ 3.0f,3.0f,3.0f }, {}, { sinf(i * 2) * 5,cosf(i * 2) * 5,200.0f }, homdmodel);

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
			setPillarDefaultPos();
			setPillarHandPos();
		}

		if (state == ophanim)
		{
			setOphanimDefaultPos();
			setOphanimHandPos();
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
		/*debugText_->SetPos(0, i * 20 + 50);
		debugText_->Printf("%d", hand[i]->getisReturnHand());*/
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

		

		if (bossBeamCount == 0 and hand[4]->getisAction() == false and waitTime == 0)
		{
			hand[4]->setTargetPos({ worldTransform.matWorldGetPos().x, worldTransform.matWorldGetPos().y - 5, worldTransform.matWorldGetPos().z+200});
			hand[4]->setisBeamFlag(true);
			bossBeamCount++;
			waitTime = bossBeamWaitTime;
			return;
		}

		if (bossBeamCount == 1 and hand[4]->getisAction() == false and waitTime == 0)
		{
			hand[4]->setTargetPos({ worldTransform.matWorldGetPos().x, worldTransform.matWorldGetPos().y - 5, worldTransform.matWorldGetPos().z + 200 });
			hand[4]->setisBeamFlag(true);

			hand[5]->setTargetPos({ worldTransform.matWorldGetPos().x - 20, worldTransform.matWorldGetPos().y - 5,worldTransform.matWorldGetPos().z + 200 });
			hand[5]->setisBeamFlag(true);
			bossBeamCount++;
			waitTime = bossBeamWaitTime;
			return;
		}

		if (bossBeamCount == 2 and hand[4]->getisAction() == false and waitTime == 0)
		{
			hand[4]->setTargetPos({ worldTransform.matWorldGetPos().x, worldTransform.matWorldGetPos().y - 5, worldTransform.matWorldGetPos().z + 200 });
			hand[4]->setisBeamFlag(true);

			hand[5]->setTargetPos({ worldTransform.matWorldGetPos().x + 20, worldTransform.matWorldGetPos().y - 5,worldTransform.matWorldGetPos().z + 200 });
			hand[5]->setisBeamFlag(true);

			hand[6]->setTargetPos({ worldTransform.matWorldGetPos().x + 40, worldTransform.matWorldGetPos().y - 5,worldTransform.matWorldGetPos().z + 200 });
			hand[6]->setisBeamFlag(true);
			bossBeamCount++;
			waitTime = bossBeamWaitTime;
			return;
		}

		if (bossBeamCount == 3 and hand[4]->getisAction() == false and waitTime == 0)
		{
			hand[4]->setTargetPos({ worldTransform.matWorldGetPos().x, worldTransform.matWorldGetPos().y - 5, worldTransform.matWorldGetPos().z + 200 });
			hand[4]->setisBeamFlag(true);

			hand[5]->setTargetPos({ worldTransform.matWorldGetPos().x + 20, worldTransform.matWorldGetPos().y - 5,worldTransform.matWorldGetPos().z + 200 });
			hand[5]->setisBeamFlag(true);

			hand[6]->setTargetPos({ worldTransform.matWorldGetPos().x + 40, worldTransform.matWorldGetPos().y - 5,worldTransform.matWorldGetPos().z + 200 });
			hand[6]->setisBeamFlag(true);

			hand[7]->setTargetPos({ worldTransform.matWorldGetPos().x - 20, worldTransform.matWorldGetPos().y - 5,worldTransform.matWorldGetPos().z + 200 });
			hand[7]->setisBeamFlag(true);
			bossBeamCount++;
			waitTime = bossBeamWaitTime;
			return;
		}

		if (bossBeamCount == 4 and hand[4]->getisAction() == false and waitTime == 0)
		{
			hand[4]->setTargetPos({ worldTransform.matWorldGetPos().x, worldTransform.matWorldGetPos().y - 5, worldTransform.matWorldGetPos().z + 200 });
			hand[4]->setisBeamFlag(true);

			hand[5]->setTargetPos({ worldTransform.matWorldGetPos().x + 20, worldTransform.matWorldGetPos().y - 5,worldTransform.matWorldGetPos().z + 200 });
			hand[5]->setisBeamFlag(true);

			hand[6]->setTargetPos({ worldTransform.matWorldGetPos().x + 40, worldTransform.matWorldGetPos().y - 5,worldTransform.matWorldGetPos().z + 200 });
			hand[6]->setisBeamFlag(true);

			hand[7]->setTargetPos({ worldTransform.matWorldGetPos().x - 20, worldTransform.matWorldGetPos().y - 5,worldTransform.matWorldGetPos().z + 200 });
			hand[7]->setisBeamFlag(true);
			bossBeamCount++;
			waitTime = bossBeamWaitTime;
			return;
		}

		if (bossBeamCount == 5 and hand[4]->getisAction() == false )
		{

			
				bossBeamCount = 0;
				
				
				isBossBeam = false;
			
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
	Vector3 bossSft;

	bossSft = { 1,1,-1 };
	bossSft = worldTransform.matWorld_.VectorMat(bossSft, worldTransform.matWorld_);
	bossSft.normalize();
	hand[0]->setTargetPos(worldTransform.matWorldGetPos() + (bossSft * setbossCubePressDistance));

	bossSft = { -1,1,-1 };
	bossSft = worldTransform.matWorld_.VectorMat(bossSft, worldTransform.matWorld_);
	bossSft.normalize();
	hand[1]->setTargetPos(worldTransform.matWorldGetPos() + (bossSft * setbossCubePressDistance));

	bossSft = { 1,-1,-1 };
	bossSft = worldTransform.matWorld_.VectorMat(bossSft, worldTransform.matWorld_);
	bossSft.normalize();
	hand[2]->setTargetPos(worldTransform.matWorldGetPos() + (bossSft * setbossCubePressDistance));

	bossSft = { -1,-1,-1 };
	bossSft = worldTransform.matWorld_.VectorMat(bossSft, worldTransform.matWorld_);
	bossSft.normalize();
	hand[3]->setTargetPos(worldTransform.matWorldGetPos() + (bossSft * setbossCubePressDistance));

	bossSft = { 1,1,1 };
	bossSft = worldTransform.matWorld_.VectorMat(bossSft, worldTransform.matWorld_);
	bossSft.normalize();
	hand[4]->setTargetPos(worldTransform.matWorldGetPos() + (bossSft * setbossCubePressDistance));

	bossSft = { -1,1,1 };
	bossSft = worldTransform.matWorld_.VectorMat(bossSft, worldTransform.matWorld_);
	bossSft.normalize();
	hand[5]->setTargetPos(worldTransform.matWorldGetPos() + (bossSft * setbossCubePressDistance));

	bossSft = { 1,-1,1 };
	bossSft = worldTransform.matWorld_.VectorMat(bossSft, worldTransform.matWorld_);
	bossSft.normalize();
	hand[6]->setTargetPos(worldTransform.matWorldGetPos() + (bossSft * setbossCubePressDistance));

	bossSft = { -1,-1,1 };
	bossSft = worldTransform.matWorld_.VectorMat(bossSft, worldTransform.matWorld_);
	bossSft.normalize();
	hand[7]->setTargetPos(worldTransform.matWorldGetPos() + (bossSft * setbossCubePressDistance));

	for (int i = 0; i < hand.size(); i++)
	{
		hand[i]->setisPressFlag(true);
	}

}

void bosstest::setPressHandPos()
{

	if (pressFirstStart)
	{
		Vector3 bossSft;
		bossSft = { 1,1,-1 };
		bossSft = worldTransform.matWorld_.VectorMat(bossSft, worldTransform.matWorld_);
		bossSft.normalize();
		hand[0]->setPos(worldTransform.matWorldGetPos() + (bossSft * setbossCubePressDistance));

		bossSft = { -1,1,-1 };
		bossSft = worldTransform.matWorld_.VectorMat(bossSft, worldTransform.matWorld_);
		bossSft.normalize();
		hand[1]->setPos(worldTransform.matWorldGetPos() + (bossSft * setbossCubePressDistance));

		bossSft = { 1,-1,-1 };
		bossSft = worldTransform.matWorld_.VectorMat(bossSft, worldTransform.matWorld_);
		bossSft.normalize();
		hand[2]->setPos(worldTransform.matWorldGetPos() + (bossSft * setbossCubePressDistance));

		bossSft = { -1,-1,-1 };
		bossSft = worldTransform.matWorld_.VectorMat(bossSft, worldTransform.matWorld_);
		bossSft.normalize();
		hand[3]->setPos(worldTransform.matWorldGetPos() + (bossSft * setbossCubePressDistance));

		bossSft = { 1,1,1 };
		bossSft = worldTransform.matWorld_.VectorMat(bossSft, worldTransform.matWorld_);
		bossSft.normalize();
		hand[4]->setPos(worldTransform.matWorldGetPos() + (bossSft * setbossCubePressDistance));

		bossSft = { -1,1,1 };
		bossSft = worldTransform.matWorld_.VectorMat(bossSft, worldTransform.matWorld_);
		bossSft.normalize();
		hand[5]->setPos(worldTransform.matWorldGetPos() + (bossSft * setbossCubePressDistance));

		bossSft = { 1,-1,1 };
		bossSft = worldTransform.matWorld_.VectorMat(bossSft, worldTransform.matWorld_);
		bossSft.normalize();
		hand[6]->setPos(worldTransform.matWorldGetPos() + (bossSft * setbossCubePressDistance));

		bossSft = { -1,-1,1 };
		bossSft = worldTransform.matWorld_.VectorMat(bossSft, worldTransform.matWorld_);
		bossSft.normalize();
		hand[7]->setPos(worldTransform.matWorldGetPos() + (bossSft * setbossCubePressDistance));

	}
	else if(hand[0]->getisPressEndd())
	{

		Vector3 bossSft;
		
		if (hand[0]->getisAction() == false)
		{
			bossSft = { 1,1,-1 };
			bossSft = worldTransform.matWorld_.VectorMat(bossSft, worldTransform.matWorld_);
			bossSft.normalize();
			hand[0]->setPos(worldTransform.matWorldGetPos() + (bossSft * setbossCubeDistance));
		}
		if (hand[1]->getisAction() == false)
		{
			bossSft = { -1,1,-1 };
			bossSft = worldTransform.matWorld_.VectorMat(bossSft, worldTransform.matWorld_);
			bossSft.normalize();
			hand[1]->setPos(worldTransform.matWorldGetPos() + (bossSft * setbossCubeDistance));
		}

		if (hand[2]->getisAction() == false)
		{

			bossSft = { 1,-1,-1 };
			bossSft = worldTransform.matWorld_.VectorMat(bossSft, worldTransform.matWorld_);
			bossSft.normalize();
			hand[2]->setPos(worldTransform.matWorldGetPos() + (bossSft * setbossCubeDistance));
		}
		
		if (hand[3]->getisAction() == false)
		{
			bossSft = { -1,-1,-1 };
			bossSft = worldTransform.matWorld_.VectorMat(bossSft, worldTransform.matWorld_);
			bossSft.normalize();
			hand[3]->setPos(worldTransform.matWorldGetPos() + (bossSft * setbossCubeDistance));
		}
		
		if (hand[4]->getisAction() == false)
		{
			bossSft = { 1,1,1 };
			bossSft = worldTransform.matWorld_.VectorMat(bossSft, worldTransform.matWorld_);
			bossSft.normalize();
			hand[4]->setPos(worldTransform.matWorldGetPos() + (bossSft * setbossCubeDistance));
		}
		
		if (hand[5]->getisAction() == false)
		{
			bossSft = { -1,1,1 };
			bossSft = worldTransform.matWorld_.VectorMat(bossSft, worldTransform.matWorld_);
			bossSft.normalize();
			hand[5]->setPos(worldTransform.matWorldGetPos() + (bossSft * setbossCubeDistance));
		}
		
		if (hand[6]->getisAction() == false)
		{
			bossSft = { 1,-1,1 };
			bossSft = worldTransform.matWorld_.VectorMat(bossSft, worldTransform.matWorld_);
			bossSft.normalize();
			hand[6]->setPos(worldTransform.matWorldGetPos() + (bossSft * setbossCubeDistance));
		}
		
		if (hand[7]->getisAction() == false)
		{
			bossSft = { -1,-1,1 };
			bossSft = worldTransform.matWorld_.VectorMat(bossSft, worldTransform.matWorld_);
			bossSft.normalize();
			hand[7]->setPos(worldTransform.matWorldGetPos() + (bossSft * setbossCubeDistance));
		}
	}
}

void bosstest::setPillarHandPos()
{
		Vector3 bossSft;
		Vector3 bossSftMove;

		//カウント
		if (bosspillarMoveTime< maxBosspillarMoveTime)
		{
			
			bosspillarMoveTime++;
		}

		//とっておいたプレイヤーの座標に移動
		if (bosspillarMoveTime < maxBosspillarMoveTime)
		{
			for (int i = 0; i < hand.size(); i++)
			{

				if (hand[i]->getisAction() == false)
				{
					
					bossSft = pillarDefaultPosRotate[(bosspillarDefaultPosCount + i) % 8];
					bossSft = worldTransform.matWorld_.VectorMat(bossSft, worldTransform.matWorld_);
					bossSft.normalize();

					bossSftMove = pillarDefaultPosRotate[(bosspillarDefaultPosCount + i + 1) % 8];
					bossSftMove = worldTransform.matWorld_.VectorMat(bossSftMove, worldTransform.matWorld_);
					bossSftMove.normalize();
					hand[i]->setPos(lerp(worldTransform.matWorldGetPos() + (bossSft * setbossCubeDistance), worldTransform.matWorldGetPos() + (bossSftMove * setbossCubeDistance), bosspillarMoveTime / maxBosspillarMoveTime));
					
				}

			}
		}

		//終了
		if (bosspillarMoveTime == maxBosspillarMoveTime)
		{
			if (bosspillarDefaultPosCount < _countof(pillarDefaultPosRotate)-1)
			{
				bosspillarDefaultPosCount++;
			}
			else
			{
				bosspillarDefaultPosCount = 0;
			}
			bosspillarMoveTime = 0;
		}


}

void bosstest::setOphanimHandPos()
{
	Vector3 bossSft;
	Vector3 bossSftMove;

	//カウント
	if (bossOphanimMoveTime < maxBossOphanimMoveTime)
	{

		bossOphanimMoveTime++;
	}

	//とっておいたプレイヤーの座標に移動
	if (bossOphanimMoveTime < maxBossOphanimMoveTime)
	{
		for (int i = 0; i < hand.size(); i++)
		{

			if (hand[i]->getisAction() == false)
			{

				bossSft = ophanimDefaultPosRotate[(bossOphanimDefaultPosCount + i) % 8];
				bossSft = worldTransform.matWorld_.VectorMat(bossSft, worldTransform.matWorld_);
				bossSft.normalize();

				bossSftMove = ophanimDefaultPosRotate[(bossOphanimDefaultPosCount + i + 1) % 8];
				bossSftMove = worldTransform.matWorld_.VectorMat(bossSftMove, worldTransform.matWorld_);
				bossSftMove.normalize();
				hand[i]->setPos(lerp(worldTransform.matWorldGetPos() + (bossSft * setbossCubeDistance), worldTransform.matWorldGetPos() + (bossSftMove * setbossCubeDistance), bossOphanimMoveTime / maxBossOphanimMoveTime));

			}

		}
	}

	//終了
	if (bossOphanimMoveTime == maxBossOphanimMoveTime)
	{
		if (bossOphanimDefaultPosCount < _countof(ophanimDefaultPosRotate) - 1)
		{
			bossOphanimDefaultPosCount++;
		}
		else
		{
			bossOphanimDefaultPosCount = 0;
		}
		bossOphanimMoveTime = 0;
	}


}

void bosstest::OnCollision(int damage)
{

	bossHP -= damage;

}


void bosstest::setCubeDefaultPos()
{

	Vector3 bossSft;

	if (hand[0]->getisAction() == false)
	{
		bossSft = { 1,1,-1 };
		bossSft = worldTransform.matWorld_.VectorMat(bossSft, worldTransform.matWorld_);
		bossSft.normalize();
		hand[0]->setdefaultPos(worldTransform.matWorldGetPos() + (bossSft * setbossCubeDistance));
	}
	if (hand[1]->getisAction() == false)
	{
		bossSft = { -1,1,-1 };
		bossSft = worldTransform.matWorld_.VectorMat(bossSft, worldTransform.matWorld_);
		bossSft.normalize();
		hand[1]->setdefaultPos(worldTransform.matWorldGetPos() + (bossSft * setbossCubeDistance));
	}

	if (hand[2]->getisAction() == false)
	{

		bossSft = { 1,-1,-1 };
		bossSft = worldTransform.matWorld_.VectorMat(bossSft, worldTransform.matWorld_);
		bossSft.normalize();
		hand[2]->setdefaultPos(worldTransform.matWorldGetPos() + (bossSft * setbossCubeDistance));
	}

	if (hand[3]->getisAction() == false)
	{
		bossSft = { -1,-1,-1 };
		bossSft = worldTransform.matWorld_.VectorMat(bossSft, worldTransform.matWorld_);
		bossSft.normalize();
		hand[3]->setdefaultPos(worldTransform.matWorldGetPos() + (bossSft * setbossCubeDistance));
	}

	if (hand[4]->getisAction() == false)
	{
		bossSft = { 1,1,1 };
		bossSft = worldTransform.matWorld_.VectorMat(bossSft, worldTransform.matWorld_);
		bossSft.normalize();
		hand[4]->setdefaultPos(worldTransform.matWorldGetPos() + (bossSft * setbossCubeDistance));
	}

	if (hand[5]->getisAction() == false)
	{
		bossSft = { -1,1,1 };
		bossSft = worldTransform.matWorld_.VectorMat(bossSft, worldTransform.matWorld_);
		bossSft.normalize();
		hand[5]->setdefaultPos(worldTransform.matWorldGetPos() + (bossSft * setbossCubeDistance));
	}

	if (hand[6]->getisAction() == false)
	{
		bossSft = { 1,-1,1 };
		bossSft = worldTransform.matWorld_.VectorMat(bossSft, worldTransform.matWorld_);
		bossSft.normalize();
		hand[6]->setdefaultPos(worldTransform.matWorldGetPos() + (bossSft * setbossCubeDistance));
	}

	if (hand[7]->getisAction() == false)
	{
		bossSft = { -1,-1,1 };
		bossSft = worldTransform.matWorld_.VectorMat(bossSft, worldTransform.matWorld_);
		bossSft.normalize();
		hand[7]->setdefaultPos(worldTransform.matWorldGetPos() + (bossSft * setbossCubeDistance));
	}

	for (int i = 0; i < hand.size(); i++)
	{

		if (hand[i]->getisAction() == false)
		{
			hand[i]->setScale({ 3.0f,3.0f,3.0f });
		}

	}

}

void bosstest::setPillarDefaultPos()
{

	Vector3 bossSft;
	Vector3 bossSftMove;
	//とっておいたプレイヤーの座標に移動
	if (bosspillarMoveTime < maxBosspillarMoveTime)
	{
		for (int i = 0; i < hand.size(); i++)
		{
			if (hand[i]->getisAction() == false)
			{
				bossSft = pillarDefaultPosRotate[(bosspillarDefaultPosCount + i) % 8];
				bossSft = worldTransform.matWorld_.VectorMat(bossSft, worldTransform.matWorld_);
				bossSft.normalize();

				bossSftMove = pillarDefaultPosRotate[(bosspillarDefaultPosCount + i + 1) % 8];
				bossSftMove = worldTransform.matWorld_.VectorMat(bossSftMove, worldTransform.matWorld_);
				bossSftMove.normalize();
				hand[i]->setdefaultPos(lerp(worldTransform.matWorldGetPos() + (bossSft * setbossCubeDistance), worldTransform.matWorldGetPos() + (bossSftMove * setbossCubeDistance), bosspillarMoveTime / maxBosspillarMoveTime));
			}
		}
	}

	

	for (int i = 0; i < hand.size(); i++)
	{

		if (hand[i]->getisAction() == false)
		{
			hand[i]->setScale({ 1.0f,10.0f,1.0f });
		}

	}

}

void bosstest::setOphanimDefaultPos()
{

	Vector3 bossSft;
	Vector3 bossSftMove;
	//とっておいたプレイヤーの座標に移動
	if (bosspillarMoveTime < maxBosspillarMoveTime)
	{
		for (int i = 0; i < hand.size(); i++)
		{
			if (hand[i]->getisAction() == false)
			{
				bossSft = ophanimDefaultPosRotate[(bosspillarDefaultPosCount + i) % 8];
				bossSft = worldTransform.matWorld_.VectorMat(bossSft, worldTransform.matWorld_);
				bossSft.normalize();

				bossSftMove = ophanimDefaultPosRotate[(bosspillarDefaultPosCount + i + 1) % 8];
				bossSftMove = worldTransform.matWorld_.VectorMat(bossSftMove, worldTransform.matWorld_);
				bossSftMove.normalize();
				hand[i]->setdefaultPos(lerp(worldTransform.matWorldGetPos() + (bossSft * setbossCubeDistance), worldTransform.matWorldGetPos() + (bossSftMove * setbossCubeDistance), bosspillarMoveTime / maxBosspillarMoveTime));
			}
		}
	}



	for (int i = 0; i < hand.size(); i++)
	{

		if (hand[i]->getisAction() == false)
		{
			hand[i]->setScale({ 2.0f,2.0f,2.0f });
		}

	}

}


void bosstest::setPressEnd()
{

	
	Vector3 bossSft;

		bossSft = { 1,1,-1 };
		bossSft = worldTransform.matWorld_.VectorMat(bossSft, worldTransform.matWorld_);
		bossSft.normalize();
		hand[0]->setdefaultPos(worldTransform.matWorldGetPos() + (bossSft * setbossCubeDistance));
	
		bossSft = { -1,1,-1 };
		bossSft = worldTransform.matWorld_.VectorMat(bossSft, worldTransform.matWorld_);
		bossSft.normalize();
		hand[1]->setdefaultPos(worldTransform.matWorldGetPos() + (bossSft * setbossCubeDistance));
	
		bossSft = { 1,-1,-1 };
		bossSft = worldTransform.matWorld_.VectorMat(bossSft, worldTransform.matWorld_);
		bossSft.normalize();
		hand[2]->setdefaultPos(worldTransform.matWorldGetPos() + (bossSft * setbossCubeDistance));

		bossSft = { -1,-1,-1 };
		bossSft = worldTransform.matWorld_.VectorMat(bossSft, worldTransform.matWorld_);
		bossSft.normalize();
		hand[3]->setdefaultPos(worldTransform.matWorldGetPos() + (bossSft * setbossCubeDistance));
	
		bossSft = { 1,1,1 };
		bossSft = worldTransform.matWorld_.VectorMat(bossSft, worldTransform.matWorld_);
		bossSft.normalize();
		hand[4]->setdefaultPos(worldTransform.matWorldGetPos() + (bossSft * setbossCubeDistance));
	
		bossSft = { -1,1,1 };
		bossSft = worldTransform.matWorld_.VectorMat(bossSft, worldTransform.matWorld_);
		bossSft.normalize();
		hand[5]->setdefaultPos(worldTransform.matWorldGetPos() + (bossSft * setbossCubeDistance));
	
		bossSft = { 1,-1,1 };
		bossSft = worldTransform.matWorld_.VectorMat(bossSft, worldTransform.matWorld_);
		bossSft.normalize();
		hand[6]->setdefaultPos(worldTransform.matWorldGetPos() + (bossSft * setbossCubeDistance));
	
		bossSft = { -1,-1,1 };
		bossSft = worldTransform.matWorld_.VectorMat(bossSft, worldTransform.matWorld_);
		bossSft.normalize();
		hand[7]->setdefaultPos(worldTransform.matWorldGetPos() + (bossSft * setbossCubeDistance));

}