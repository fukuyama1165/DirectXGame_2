#include "bossHand.h"
#include <cassert>

const Vector3 lerp(const Vector3& start, const Vector3& end, const float t);


void bossHand::init(Vector3 scale, Vector3 rotate, Vector3 translation, Model* model)
{

	assert(model);
	this->model = model;

	debugText_ = DebugText::GetInstance();

	Hand.Initialize();

	Hand.scale_ = scale;
	Hand.rotation_ = rotate;
	Hand.translation_ = translation;

	Hand.matWorldGeneration();
}

void bossHand::update(WorldTransform worldTransform)
{

	Hand.matWorldGeneration();

	punch(worldTransform);

}

void bossHand::draw(const ViewProjection& viewProjection)
{

	model->Draw(Hand, viewProjection);

}

void bossHand::punch(WorldTransform worldTransform)
{

	if (!isReturnHand)
	{
		if (isAttackFlag)
		{

			if (timeCount < maxTime and waitTime > attackWaitTime)
			{
				timeCount++;
			}

			if (waitTime < attackWaitTime and isAttackReturnFlag == false)
			{

				Hand.translation_ = { cosf(waitTime) + defaultPos.x, defaultPos.y, defaultPos.z };

				Hand.matWorldGeneration();

			}

			if (timeCount != maxTime and isAttackReturnFlag == false and waitTime > attackWaitTime)
			{


				//元の位置からプレイヤーの位置(現在は0,0,0最終的に狙いを決定してからそこの位置へ)に線形補間
				Hand.translation_ = lerp(defaultPos, targetPos, timeCount / maxTime);

				Hand.matWorldGeneration();
			}
			else if (isAttackReturnFlag == false and waitTime > attackWaitTime)
			{
				isAttackReturnFlag = true;
				waitTime = 0;
			}

			if (isAttackReturnFlag and returnTimeCount < maxReturnTime and waitTime > returnWaitTime)
			{
				returnTimeCount++;
			}

			if (waitTime > 80 and waitTime < returnWaitTime and isAttackReturnFlag)
			{

				Hand.translation_ = { cosf(waitTime) / 10 + targetPos.x,  targetPos.y,  targetPos.z };

				Hand.matWorldGeneration();

			}

			if (isAttackReturnFlag and returnTimeCount != maxReturnTime and waitTime > returnWaitTime)
			{

				Hand.translation_ = lerp(targetPos, defaultPos, returnTimeCount / maxReturnTime);
				Hand.matWorldGeneration();
			}

			if (returnTimeCount == maxReturnTime)
			{
				punchEnd();
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

		Hand.translation_ = lerp(returnPos, Vector3(worldTransform.translation_.x, worldTransform.translation_.y, worldTransform.translation_.z), returnAttackTimeCount / maxReturnAttackTime);
		Hand.matWorldGeneration();

		if (returnAttackTimeCount == maxReturnAttackTime)
		{
			Hand.translation_ = { defaultPos};
			Hand.matWorldGeneration();
			punchEnd();
		}


	}

	debugText_->SetPos(50, 90);
	debugText_->Printf("leftHandpos:(%f,%f,%f)", Hand.translation_.x, Hand.translation_.y, Hand.translation_.z);

}

void bossHand::setisAttackFlag(bool flag)
{
	isAttackFlag = flag;
}


void bossHand::playerAttackReturn()
{

	isReturnHand = true;
	returnPos = Hand.matWorldGetPos();

}

void bossHand::punchEnd()
{
	isReturnHand = false;
	isAttackFlag = false;
	isAttackReturnFlag = false;
	returnAttackTimeCount = 0;
	timeCount = 0;
	returnTimeCount = 0;
	waitTime = 0;

}

const Vector3 lerp(const Vector3& start, const Vector3& end, const float t)
{
	return start + t * (end - start);
}