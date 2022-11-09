#include "bosstest.h"
#include <cassert>
#include <cstdlib>

const Vector3 lerp(const Vector3& start, const Vector3& end, const float t);

//単純に!=
bool vector3IsDiffer(Vector3 a, Vector3 b);
//単純に==
bool vector3Issame(Vector3 a, Vector3 b);

bosstest::bosstest()
{

}
bosstest::~bosstest()
{

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

	leftHand.Initialize();

	rightHand.Initialize();

	/*leftHand.parent_ = &worldTransform;
	rightHand.parent_ = &worldTransform;*/

	leftHand.scale_ = { 1.0f,1.0f,1.0f };
	leftHand.translation_ = { 4.0f,0,200 };

	leftHand.matWorldGeneration();

	rightHand.scale_ = { 1.0f,1.0f,1.0f };
	rightHand.translation_ = { -4.0f,-0.5f,200 };

	rightHand.matWorldGeneration();


}

void bosstest::Update()
{

	

	worldTransform.matWorldGeneration();
	leftHand.matWorldGeneration();
	rightHand.matWorldGeneration();

	if (!isReturnHandL)
	{
		if (isAttackFlagL)
		{

			if (timeCount < maxTime and waitTime > attackWaitTime)
			{
				timeCount++;
			}

			if (waitTime < attackWaitTime and isAttackReturnFlagL == false)
			{

				leftHand.translation_ = { cosf(waitTime) + worldTransform.translation_.x + 4.0f, worldTransform.translation_.y, worldTransform.translation_.z };

				leftHand.matWorldGeneration();

			}

			if ( timeCount!=maxTime and isAttackReturnFlagL==false and waitTime > attackWaitTime)
			{


				//元の位置からプレイヤーの位置(現在は0,0,0最終的に狙いを決定してからそこの位置へ)に線形補間
				leftHand.translation_ = lerp(Vector3(worldTransform.translation_.x + 4.0f, worldTransform.translation_.y, worldTransform.translation_.z), targetPos,timeCount/ maxTime);
				
				leftHand.matWorldGeneration();
			}
			else if (isAttackReturnFlagL == false and waitTime > attackWaitTime)
			{
				isAttackReturnFlagL = true;
				waitTime = 0;
			}

			if (isAttackReturnFlagL and returnTimeCount < maxReturnTime and waitTime > returnWaitTime)
			{
				returnTimeCount++;
			}

			if (waitTime > 80 and waitTime < returnWaitTime and isAttackReturnFlagL)
			{

				leftHand.translation_ = { cosf(waitTime)/10 + targetPos.x,  targetPos.y,  targetPos.z };

				leftHand.matWorldGeneration();

			}

			if (isAttackReturnFlagL and returnTimeCount != maxReturnTime and waitTime > returnWaitTime)
			{

				leftHand.translation_ = lerp(targetPos, Vector3(worldTransform.translation_.x + 4.0f, worldTransform.translation_.y, worldTransform.translation_.z), returnTimeCount / maxReturnTime);
				leftHand.matWorldGeneration();
			}

			if (returnTimeCount == maxReturnTime)
			{
				attackEnd();
			}

			waitTime++;

		}
	}
	else
	{

		if (returnAttackTimeCount < maxReturnAttackTime)
		{
			returnAttackTimeCount++;
		}

		leftHand.translation_ = lerp(returnPos, Vector3(worldTransform.translation_.x, worldTransform.translation_.y, worldTransform.translation_.z), returnAttackTimeCount / maxReturnAttackTime);
		leftHand.matWorldGeneration();

		if (returnAttackTimeCount == maxReturnAttackTime)
		{
			leftHand.translation_ = { worldTransform.translation_.x + 4.0f, worldTransform.translation_.y, worldTransform.translation_.z };
			leftHand.matWorldGeneration();
			attackEnd();
		}


	}

	

	debugText_->SetPos(50, 90);
	debugText_->Printf("leftHandpos:(%f,%f,%f)", leftHand.translation_.x, leftHand.translation_.y, leftHand.translation_.z);
}

void bosstest::Draw(const ViewProjection& viewProjection)
{
	model_->Draw(leftHand, viewProjection);
	model_->Draw(rightHand, viewProjection);
	model_->Draw(worldTransform, viewProjection);
	
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
	isAttackFlagL = flag;
}

void bosstest::setisAttackFlagR(bool flag)
{
	isAttackFlagR = flag;
}

void bosstest::playerAttackReturnL()
{

	isReturnHandL = true;
	returnPos = leftHand.matWorldGetPos();

}

void bosstest::attackEnd()
{
	isReturnHandL = false;
	isAttackFlagL = false;
	isAttackReturnFlagL = false;
	returnAttackTimeCount = 0;
	timeCount = 0;
	returnTimeCount = 0;
	waitTime = 0;

}

const Vector3 lerp(const Vector3& start, const Vector3& end, const float t)
{
	return start +  t * (end - start);
}

bool vector3IsDiffer(Vector3 a, Vector3 b)
{
	if (a.x != b.x or a.y!=b.y or a.z!=b.z)
	{
		return true;
	}

	return false;
}

bool vector3Issame(Vector3 a, Vector3 b)
{

	if (a.x == b.x and a.y == b.y and a.z == b.z)
	{
		return true;
	}

	return false;

}
