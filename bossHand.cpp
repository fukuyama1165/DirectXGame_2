#include "bossHand.h"
#include <cassert>

//�P����!=
bool vector3IsDiffer(Vector3 a, Vector3 b);

//�P����==
bool vector3Issame(Vector3 a, Vector3 b);

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
	press();
	stoneFall();
	beam();

}

void bossHand::draw(const ViewProjection& viewProjection)
{

	model->Draw(Hand, viewProjection);

}

void bossHand::punch(WorldTransform worldTransform)
{

	if (isReturnHand==false)
	{
		if (isAttackFlag)
		{
			isAction = true;
			//�J�E���g
			if (timeCount < maxTime and waitTime > attackWaitTime)
			{
				timeCount++;
			}

			//�\������
			if (waitTime < attackWaitTime and isAttackReturnFlag == false)
			{

				Hand.translation_ = { cosf(waitTime) + defaultPos.x, defaultPos.y, defaultPos.z };

				Hand.matWorldGeneration();

			}

			//�U��
			if (timeCount != maxTime and isAttackReturnFlag == false and waitTime > attackWaitTime)
			{


				//���̈ʒu����v���C���[�̈ʒu(���݂�0,0,0�ŏI�I�ɑ_�������肵�Ă��炻���̈ʒu��)�ɐ��`���
				Hand.translation_ = lerp(defaultPos, targetPos, timeCount / maxTime);

				Hand.matWorldGeneration();
			}
			else if (isAttackReturnFlag == false and waitTime > attackWaitTime)
			{
				isAttackReturnFlag = true;
				waitTime = 0;
			}

			//�J�E���g
			if (isAttackReturnFlag and returnTimeCount < maxReturnTime and waitTime > returnWaitTime)
			{
				returnTimeCount++;
			}

			//�A�҂̗\������(80�͓����Ȃ�����)
			if (waitTime > 80 and waitTime < returnWaitTime and isAttackReturnFlag)
			{

				Hand.translation_ = { cosf(waitTime) / 10 + targetPos.x,  targetPos.y,  targetPos.z };

				Hand.matWorldGeneration();

			}

			//�A��
			if (isAttackReturnFlag and returnTimeCount != maxReturnTime and waitTime > returnWaitTime)
			{

				Hand.translation_ = lerp(targetPos, defaultPos, returnTimeCount / maxReturnTime);
				Hand.matWorldGeneration();
			}

			waitTime++;

			//�I��
			if (returnTimeCount == maxReturnTime)
			{
				punchEnd();
			}

			

		}
	}

	//�U���ɂ���ĕԂ��ꂽ��
	if(isReturnHand)
	{

		//�J�E���g
		if (returnAttackTimeCount < maxReturnAttackTime)
		{
			returnAttackTimeCount++;
		}

		//�{�̂̂��Ƃ֋A��
		Hand.translation_ = lerp(returnPos, Vector3(worldTransform.translation_.x, worldTransform.translation_.y, worldTransform.translation_.z), returnAttackTimeCount / maxReturnAttackTime);
		Hand.matWorldGeneration();

		//�{�̂ɓ��������猳�̈ʒu��
		if (returnAttackTimeCount == maxReturnAttackTime)
		{
			Hand.translation_ = { defaultPos};
			Hand.matWorldGeneration();
			punchEnd();
		}


	}

	/*debugText_->SetPos(50, 90);
	debugText_->Printf("leftHandpos:(%f,%f,%f)", Hand.translation_.x, Hand.translation_.y, Hand.translation_.z);*/

}

void bossHand::press()
{
	if (isPress)
	{
		isAction = true;
		isPressEnd = false;
		if (timeCount < maxSetPressTime)
		{
			timeCount++;
		}

		if (timeCount != maxSetPressTime)
		{


			//���̈ʒu����v���C���[�̈ʒu(���݂�0,0,0�ŏI�I�ɑ_�������肵�Ă��炻���̈ʒu��)�ɐ��`���
			Hand.translation_ = lerp(defaultPos, targetPos, timeCount / maxSetPressTime);

			Hand.matWorldGeneration();
		}
	}
	else
	{

		if (vector3IsDiffer(defaultPos, Hand.matWorldGetPos()) and isPressEnd ==false)
		{

			if (returnTimeCount < maxResetPressTime)
			{
				returnTimeCount++;
			}

			//���̈ʒu����v���C���[�̈ʒu(���݂�0,0,0�ŏI�I�ɑ_�������肵�Ă��炻���̈ʒu��)�ɐ��`���
			Hand.translation_ = lerp(targetPos, defaultPos, returnTimeCount / maxResetPressTime);

			Hand.matWorldGeneration();

			

		}

		if (!vector3IsDiffer(defaultPos, Hand.matWorldGetPos()) and isPressEnd == false)
		{

			if (returnTimeCount == maxResetPressTime)
			{
				returnTimeCount = 0;
			}

			isPressEnd = true;

			timeCount = 0;

			isAction = false;

		}



	}

}

void bossHand::stoneFall()
{

	if (isStoneFall)
	{
		isAction = true;
		//�J�E���g
		if (timeCount < maxUpFallTime and waitTime > stoneFallWaitTime and isFallTargetMoveFlag == false)
		{
			timeCount++;
		}

		//�\������
		if (waitTime < stoneFallWaitTime and isFallTargetMoveFlag == false)
		{

			Hand.translation_ = { cosf(waitTime) + defaultPos.x, defaultPos.y, defaultPos.z };

			Hand.matWorldGeneration();

		}

		//��Ɉړ�
		if (timeCount < maxUpFallTime and isFallTargetMoveFlag == false and waitTime > stoneFallWaitTime)
		{


			//���̈ʒu��y���W�����炵���ʒu�ɐ��`���
			Hand.translation_ = lerp(defaultPos, {defaultPos.x,15,defaultPos.z}, timeCount / maxUpFallTime);

			Hand.matWorldGeneration();
		}
		//��Ɉړ����I�������
		else if (isFallTargetMoveFlag == false and waitTime > stoneFallWaitTime)
		{
			isFallTargetMoveFlag = true;
			waitTime = 0;
		}

		//�v���C���[�̍��W�v��
		if (isFallTargetMoveFlag and returnTimeCount == 0 and isGetTargetPos == false)
		{
			isGetTargetPos = true;
			return;
		}
		else if (isFallTargetMoveFlag and returnTimeCount != 0)
		{
			isGetTargetPos = false;
		}


		//�J�E���g
		if (isFallTargetMoveFlag and returnTimeCount < maxTargetMoveTime and isFallFallFlag ==false)
		{
			//�g���܂킵���O�ɈӖ��͂Ȃ�
			returnTimeCount++;
		}

		//�Ƃ��Ă������v���C���[�̍��W�Ɉړ�
		if (isFallTargetMoveFlag and returnTimeCount != maxTargetMoveTime and isFallFallFlag == false)
		{

			Hand.translation_ = lerp({ defaultPos.x,15,defaultPos.z }, {targetPos.x,15,targetPos.z}, returnTimeCount / maxTargetMoveTime);
			Hand.matWorldGeneration();
		}

		//�I��
		if (returnTimeCount == maxTargetMoveTime and isFallFallFlag == false)
		{
			isFallFallFlag = true;
			waitTime = 0;
		}

		//�J�E���g
		if (ActionType4TimeCount < maxFallTime and isFallFallFlag and isFallReturnFlag == false)
		{
			ActionType4TimeCount++;
		}

		//����
		if (ActionType4TimeCount < maxFallTime and isFallFallFlag and isFallReturnFlag == false)
		{


			//���Ɉړ�
			Hand.translation_ = lerp({ targetPos.x,15,targetPos.z }, {targetPos.x,targetPos.y+1.1f,targetPos.z}, ActionType4TimeCount / maxFallTime);

			Hand.matWorldGeneration();
		}

		if (ActionType4TimeCount == maxFallTime and isFallReturnFlag == false)
		{
			isFallReturnFlag = true;
			waitTime = 0;
		}


		//���S�Ɏ~�߂������Ԃ�300
		if (waitTime > 150 and returnAttackTimeCount < maxFallReturnTime and waitTime > (stoneFallReturnWaitTime +150) and isFallReturnFlag)
		{
			returnAttackTimeCount++;
		}

		//�\������
		if (waitTime > 150 and waitTime < (stoneFallReturnWaitTime + 150) and isFallReturnFlag)
		{

			Hand.translation_ = { cosf(waitTime) + targetPos.x, targetPos.y+1.8f, targetPos.z };

			Hand.matWorldGeneration();

		}

		//��Ɉړ�
		if (returnAttackTimeCount < maxFallReturnTime and isFallReturnFlag and waitTime > (stoneFallReturnWaitTime + 150))
		{


			//���̈ʒu��y���W�����炵���ʒu�ɐ��`���
			Hand.translation_ = lerp(targetPos, defaultPos, returnAttackTimeCount / maxFallReturnTime);

			Hand.matWorldGeneration();
		}


		waitTime++;

		if (returnAttackTimeCount == maxFallReturnTime)
		{
			
			isAction = false;
			isFallTargetMoveFlag = false;
			isFallFallFlag = false;
			isFallReturnFlag = false;

			timeCount = 0;
			returnTimeCount = 0;
			ActionType4TimeCount = 0;
			returnAttackTimeCount = 0;

			waitTime = 0;

			isStoneFall = false;

		}

	}

}

void bossHand::beam()
{

	if (isBeam)
	{

		isAction = true;


		if (isBeamFirstStart == false)
		{

			Hand.scale_ = startBeamScale;

			Hand.matWorldGeneration();
			isBeamFirstStart = true;

		}

		//�J�E���g
		if (timeCount < maxBeamTime and waitTime >60)
		{
			timeCount++;
		}

		if (timeCount != maxBeamTime)
		{

			Hand.translation_ = lerp(targetPos, { targetPos.x,targetPos.y,targetPos.z-600 }, timeCount / maxBeamTime);
			Hand.matWorldGeneration();
		}

		//�I��

		if (timeCount == maxBeamTime)
		{
			Hand.scale_ = { 1.0f,1.0f,1.0f };
			Hand.matWorldGeneration();

			isAction = false;
			isBeamFirstStart = false;
			isBeamEnd = false;
			isBeam = false;

			timeCount = 0;
			waitTime = 0;
		}

		waitTime++;

	}




}

void bossHand::setisAttackFlag(bool flag)
{
	isAttackFlag = flag;
}

void bossHand::setisPressFlag(bool flag)
{
	isPress = flag;
}

void bossHand::setisPressEndFlag(bool flag)
{
	isPressEnd = flag;
}

void bossHand::setisStoneFallFlag(bool flag)
{
	isStoneFall = flag;
}

void  bossHand::setisBeamFlag(bool flag)
{
	isBeam = flag;
}


void bossHand::playerAttackReturn()
{

	isReturnHand = true;
	returnPos = Hand.matWorldGetPos();

}

void bossHand::punchEnd()
{
	isAction = false;
	isReturnHand = false;
	isAttackFlag = false;
	isAttackReturnFlag = false;
	returnAttackTimeCount = 0;
	timeCount = 0;
	returnTimeCount = 0;
	waitTime = 0;

}

void bossHand::setPos(Vector3 pos)
{
	Hand.translation_ = pos;

	Hand.matWorldGeneration();
}

void bossHand::setScale(Vector3 scale)
{

	Hand.scale_ = scale;

	Hand.matWorldGeneration();

}

void bossHand::setTargetPos(Vector3 target)
{
	targetPos = target;
}

const Vector3 lerp(const Vector3& start, const Vector3& end, const float t)
{
	return start + t * (end - start);
}

bool vector3IsDiffer(Vector3 a, Vector3 b)
{
	if (a.x != b.x or a.y != b.y or a.z != b.z)
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