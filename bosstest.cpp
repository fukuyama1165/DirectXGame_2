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

			if (timeCount < maxTime)
			{
				timeCount++;
			}

			if ( timeCount!=maxTime and isAttackReturnFlagL==false)
			{

				Vector3 bossplayer = { 0 - (worldTransform.translation_.x + 2.0f),0 - worldTransform.translation_.y ,0-worldTransform.translation_.z };

				bossplayer.normalize();

				//元の位置からプレイヤーの位置(現在は0,0,0最終的に狙いを決定してからそこの位置へ)に線形補間
				leftHand.translation_.z += bossplayer.z * 1;
				
				leftHand.matWorldGeneration();
			}
			else if (isAttackReturnFlagL == false)
			{
				isAttackReturnFlagL = true;
				timeCount = 0;
			}

			if (isAttackReturnFlagL and returnTimeCount < maxReturnTime)
			{
				returnTimeCount++;
			}

			if (isAttackReturnFlagL and returnTimeCount != maxReturnTime)
			{
				Vector3 bossplayer = { worldTransform.translation_.x + 1.5f - 0,worldTransform.translation_.y - 0,worldTransform.translation_.z - 1 };

				bossplayer.normalize();

				leftHand.translation_.z += bossplayer.z * 1;
				leftHand.matWorldGeneration();
			}

		}
	}
	else
	{



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
