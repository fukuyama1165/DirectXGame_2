#include "bossHand.h"
#include <cassert>

//単純に!=
bool vector3IsDiffer(Vector3 a, Vector3 b);

//単純に==
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
	pillarFall();
	pillarRoll();
	pillarPushUp();

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
			//カウント
			if (timeCount < maxTime and waitTime > attackWaitTime)
			{
				timeCount++;
			}

			//予備動作
			if (waitTime < attackWaitTime and isAttackReturnFlag == false)
			{

				Hand.translation_ = { cosf(waitTime) + defaultPos.x, defaultPos.y, defaultPos.z };

				Hand.matWorldGeneration();

			}

			//攻撃
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

			//カウント
			if (isAttackReturnFlag and returnTimeCount < maxReturnTime and waitTime > returnWaitTime)
			{
				returnTimeCount++;
			}

			//帰還の予備動作(80は動かない時間)
			if (waitTime > 80 and waitTime < returnWaitTime and isAttackReturnFlag)
			{

				Hand.translation_ = { cosf(waitTime) / 10 + targetPos.x,  targetPos.y,  targetPos.z };

				Hand.matWorldGeneration();

			}

			//帰還
			if (isAttackReturnFlag and returnTimeCount != maxReturnTime and waitTime > returnWaitTime)
			{

				Hand.translation_ = lerp(targetPos, defaultPos, returnTimeCount / maxReturnTime);
				Hand.matWorldGeneration();
			}

			waitTime++;

			//終了
			if (returnTimeCount == maxReturnTime)
			{
				punchEnd();
			}

			

		}
	}

	//攻撃によって返された時
	if(isReturnHand)
	{

		//カウント
		if (returnAttackTimeCount < maxReturnAttackTime)
		{
			returnAttackTimeCount++;
		}

		//本体のもとへ帰る
		Hand.translation_ = lerp(returnPos, Vector3(worldTransform.translation_.x, worldTransform.translation_.y, worldTransform.translation_.z), returnAttackTimeCount / maxReturnAttackTime);
		Hand.matWorldGeneration();

		//本体に当たったら元の位置へ
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


			//元の位置からプレイヤーの位置(現在は0,0,0最終的に狙いを決定してからそこの位置へ)に線形補間
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

			//元の位置からプレイヤーの位置(現在は0,0,0最終的に狙いを決定してからそこの位置へ)に線形補間
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
		//カウント
		if (timeCount < maxUpFallTime and waitTime > stoneFallWaitTime and isFallTargetMoveFlag == false)
		{
			timeCount++;
		}

		//予備動作
		if (waitTime < stoneFallWaitTime and isFallTargetMoveFlag == false)
		{

			Hand.translation_ = { cosf(waitTime) + defaultPos.x, defaultPos.y, defaultPos.z };

			Hand.matWorldGeneration();

		}

		//上に移動
		if (timeCount < maxUpFallTime and isFallTargetMoveFlag == false and waitTime > stoneFallWaitTime)
		{


			//元の位置かy座標をずらした位置に線形補間
			Hand.translation_ = lerp(defaultPos, {defaultPos.x,15,defaultPos.z}, timeCount / maxUpFallTime);

			Hand.matWorldGeneration();
		}
		//上に移動が終わったら
		else if (isFallTargetMoveFlag == false and waitTime > stoneFallWaitTime)
		{
			isFallTargetMoveFlag = true;
			waitTime = 0;
		}

		//プレイヤーの座標要求
		if (isFallTargetMoveFlag and returnTimeCount == 0 and isGetTargetPos == false)
		{
			isGetTargetPos = true;
			return;
		}
		else if (isFallTargetMoveFlag and returnTimeCount != 0)
		{
			isGetTargetPos = false;
		}


		//カウント
		if (isFallTargetMoveFlag and returnTimeCount < maxTargetMoveTime and isFallFallFlag ==false)
		{
			//使いまわし名前に意味はない
			returnTimeCount++;
		}

		//とっておいたプレイヤーの座標に移動
		if (isFallTargetMoveFlag and returnTimeCount != maxTargetMoveTime and isFallFallFlag == false)
		{

			Hand.translation_ = lerp({ defaultPos.x,15,defaultPos.z }, {targetPos.x,15,targetPos.z}, returnTimeCount / maxTargetMoveTime);
			Hand.matWorldGeneration();
		}

		//終了
		if (returnTimeCount == maxTargetMoveTime and isFallFallFlag == false)
		{
			isFallFallFlag = true;
			waitTime = 0;
		}

		//カウント
		if (ActionType4TimeCount < maxFallTime and isFallFallFlag and isFallReturnFlag == false)
		{
			ActionType4TimeCount++;
		}

		//落下
		if (ActionType4TimeCount < maxFallTime and isFallFallFlag and isFallReturnFlag == false)
		{


			//下に移動
			Hand.translation_ = lerp({ targetPos.x,15,targetPos.z }, {targetPos.x,targetPos.y+1.1f,targetPos.z}, ActionType4TimeCount / maxFallTime);

			Hand.matWorldGeneration();
		}

		if (ActionType4TimeCount == maxFallTime and isFallReturnFlag == false)
		{
			isFallReturnFlag = true;
			waitTime = 0;
		}


		//完全に止めたい時間が300
		if (waitTime > 150 and returnAttackTimeCount < maxFallReturnTime and waitTime > (stoneFallReturnWaitTime +150) and isFallReturnFlag)
		{
			returnAttackTimeCount++;
		}

		//予備動作
		if (waitTime > 150 and waitTime < (stoneFallReturnWaitTime + 150) and isFallReturnFlag)
		{

			Hand.translation_ = { cosf(waitTime) + targetPos.x, targetPos.y+1.8f, targetPos.z };

			Hand.matWorldGeneration();

		}

		//上に移動
		if (returnAttackTimeCount < maxFallReturnTime and isFallReturnFlag and waitTime > (stoneFallReturnWaitTime + 150))
		{


			//元の位置かy座標をずらした位置に線形補間
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

void bossHand::pillarFall()
{

	if (isPillarFall)
	{
		isAction = true;
		//カウント
		if (timeCount < maxUpFallTime and waitTime > stoneFallWaitTime and isFallTargetMoveFlag == false)
		{
			timeCount++;
		}

		//予備動作
		if (waitTime < stoneFallWaitTime and isFallTargetMoveFlag == false)
		{

			Hand.translation_ = { cosf(waitTime) + defaultPos.x, defaultPos.y, defaultPos.z };

			Hand.matWorldGeneration();

		}

		//上に移動
		if (timeCount < maxUpFallTime and isFallTargetMoveFlag == false and waitTime > stoneFallWaitTime)
		{


			//元の位置かy座標をずらした位置に線形補間
			Hand.translation_ = lerp(defaultPos, { defaultPos.x,30,defaultPos.z }, timeCount / maxUpFallTime);

			Hand.matWorldGeneration();
		}
		//上に移動が終わったら
		else if (isFallTargetMoveFlag == false and waitTime > stoneFallWaitTime)
		{
			isFallTargetMoveFlag = true;
			waitTime = 0;
		}

		//プレイヤーの座標要求
		if (isFallTargetMoveFlag and returnTimeCount == 0 and isGetTargetPos == false)
		{
			isGetTargetPos = true;
			return;
		}
		else if (isFallTargetMoveFlag and returnTimeCount != 0)
		{
			isGetTargetPos = false;
		}


		//カウント
		if (isFallTargetMoveFlag and returnTimeCount < maxTargetMoveTime and isFallFallFlag == false)
		{
			//使いまわし名前に意味はない
			returnTimeCount++;
		}

		//とっておいたプレイヤーの座標に移動
		if (isFallTargetMoveFlag and returnTimeCount != maxTargetMoveTime and isFallFallFlag == false)
		{

			Hand.translation_ = lerp({ defaultPos.x,30,defaultPos.z }, { targetPos.x,30,targetPos.z }, returnTimeCount / maxTargetMoveTime);
			Hand.matWorldGeneration();
		}

		//終了
		if (returnTimeCount == maxTargetMoveTime and isFallFallFlag == false)
		{
			isFallFallFlag = true;
			waitTime = 0;
		}

		//カウント
		if (ActionType4TimeCount < maxFallTime and isFallFallFlag and isPillarFallReturnFlag == false)
		{
			ActionType4TimeCount++;
		}

		//落下
		if (ActionType4TimeCount < maxFallTime and isFallFallFlag and isPillarFallReturnFlag == false)
		{


			//下に移動
			Hand.translation_ = lerp({ targetPos.x,30,targetPos.z }, { targetPos.x,targetPos.y + 6.1f,targetPos.z }, ActionType4TimeCount / maxFallTime);

			Hand.matWorldGeneration();
		}

		if (ActionType4TimeCount == maxFallTime and isPillarFallReturnFlag == false)
		{
			isPillarFallReturnFlag = true;
			waitTime = 0;
		}


		//完全に止めたい時間が150
		if (waitTime > 150 and ActionType5TimeCount < maxUpPillarFallTime and waitTime >(stoneFallReturnWaitTime + 150) and isPillarFallReturnFlag and isFallReturnFlag==false)
		{
			ActionType5TimeCount++;
		}

		//予備動作
		if (waitTime > 150 and waitTime < (stoneFallReturnWaitTime + 150) and isPillarFallReturnFlag and isFallReturnFlag == false)
		{

			Hand.translation_ = { cosf(waitTime) + targetPos.x, targetPos.y + 6.8f, targetPos.z };

			Hand.matWorldGeneration();

		}

		//上に移動
		if (ActionType5TimeCount < maxUpPillarFallTime and isPillarFallReturnFlag and waitTime >(stoneFallReturnWaitTime + 150) and isFallReturnFlag == false)
		{


			//元の位置かy座標をずらした位置に線形補間
			Hand.translation_ = lerp({ targetPos.x,targetPos.y + 11.1f,targetPos.z }, { targetPos.x,15,targetPos.z }, ActionType5TimeCount / maxUpPillarFallTime);

			Hand.matWorldGeneration();
		}

		if (ActionType5TimeCount == maxUpPillarFallTime and isFallReturnFlag == false)
		{
			isFallReturnFlag = true;
			waitTime = 0;
		}

		//カウント
		if (returnAttackTimeCount < maxFallReturnTime and isFallReturnFlag)
		{
			returnAttackTimeCount++;
		}

		//戻る
		if (returnAttackTimeCount < maxFallReturnTime and isFallReturnFlag)
		{


			//下に移動
			Hand.translation_ = lerp({ targetPos.x,30,targetPos.z }, defaultPos, returnAttackTimeCount / maxFallReturnTime);

			Hand.matWorldGeneration();
		}


		waitTime++;

		if (returnAttackTimeCount == maxFallReturnTime)
		{

			isAction = false;
			isFallTargetMoveFlag = false;
			isFallFallFlag = false;
			isFallReturnFlag = false;
			isPillarFallReturnFlag = false;

			timeCount = 0;
			returnTimeCount = 0;
			ActionType4TimeCount = 0;
			ActionType5TimeCount = 0;
			returnAttackTimeCount = 0;

			waitTime = 0;

			isPillarFall = false;

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

			//現在位置取得
			returnPos = Hand.matWorldGetPos();

			Hand.matWorldGeneration();
			isBeamFirstStart = true;

		}
		else if(returnTimeCount < maxFirstBeamTime)
		{

			returnTimeCount++;
			Hand.translation_ = lerp(returnPos, targetPos, returnTimeCount / maxFirstBeamTime);
			Hand.matWorldGeneration();

		}


		//カウント
		if (timeCount < maxBeamTime and waitTime > 15)
		{
			timeCount++;
		}

		if (timeCount != maxBeamTime and waitTime > 15)
		{

			Hand.translation_ = lerp(targetPos, { targetPos.x,targetPos.y,targetPos.z-600 }, timeCount / maxBeamTime);
			Hand.matWorldGeneration();
		}

		//終了

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
			returnTimeCount = 0;
		}

		if (returnTimeCount == maxFirstBeamTime)
		{
			waitTime++;
		}

	}




}

void bossHand::pillarRoll()
{
	if (isPillarRoll)
	{

		if (isPillarRollFirstStart == false)
		{
			returnPos = Hand.matWorldGetPos();
			isPillarRollFirstStart = true;
		}

		if (timeCount < maxPillarRollTime)
		{
			timeCount++;
		}

		if (timeCount < maxPillarRollTime)
		{


			//元の位置からプレイヤーの位置(現在は0,0,0最終的に狙いを決定してからそこの位置へ)に線形補間
			Hand.translation_ = lerp(returnPos, defaultPos, timeCount / maxPillarRollTime);

			Hand.matWorldGeneration();
		}

		if (timeCount == maxPillarRollTime)
		{

			isPillarRollFirstStart = false;
			timeCount = 0;
			isAction = false;
			isPillarRoll = false;
		}

	}
}

void bossHand::pillarPushUp()
{

	if (isPillarPushUp)
	{
		isAction = true;
		//カウント
		if (timeCount < maxPushUpFallTime and waitTime > PushUpWaitTime and isPushUpTargetMoveFlag == false)
		{
			timeCount++;
		}

		//予備動作
		if (waitTime < maxPushUpFallTime and isPushUpTargetMoveFlag == false)
		{

			Hand.translation_ = { cosf(waitTime) + defaultPos.x, defaultPos.y, defaultPos.z };

			Hand.matWorldGeneration();

		}

		//下に移動
		if (timeCount < maxUpFallTime and isPushUpTargetMoveFlag == false and waitTime > PushUpWaitTime)
		{


			//元の位置かy座標をずらした位置に線形補間
			Hand.translation_ = lerp(defaultPos, { defaultPos.x,-12,defaultPos.z }, timeCount / maxPushUpFallTime);

			Hand.matWorldGeneration();
		}
		//下に移動が終わったら
		else if (isPushUpTargetMoveFlag == false and waitTime > PushUpWaitTime)
		{
			isPushUpTargetMoveFlag = true;
			waitTime = 0;
		}

		//カウント
		if (isPushUpTargetMoveFlag and returnTimeCount < maxPushUpTargetMoveTime and isPushUpUpFlag == false)
		{
			//使いまわし名前に意味はない
			returnTimeCount++;
		}

		//とっておいたプレイヤーの座標に移動
		if (isPushUpTargetMoveFlag and returnTimeCount < maxPushUpTargetMoveTime and isPushUpUpFlag == false)
		{

			Hand.translation_ = lerp({ defaultPos.x,-12,defaultPos.z }, { targetPos.x,-12,targetPos.z }, returnTimeCount / maxPushUpTargetMoveTime);
			Hand.matWorldGeneration();
		}

		//終了
		if (returnTimeCount == maxPushUpTargetMoveTime and isPushUpUpFlag == false)
		{
			isPushUpUpFlag = true;
		}

		//カウント
		if (ActionType4TimeCount < maxPushUpUpTime and isPushUpUpFlag and isPushUpFallFlag == false)
		{
			ActionType4TimeCount++;
		}

		//落下
		if (ActionType4TimeCount < maxPushUpUpTime and isPushUpUpFlag and isPushUpFallFlag == false)
		{


			//下に移動
			Hand.translation_ = lerp({ targetPos.x,-12,targetPos.z }, { targetPos.x,targetPos.y + 6.1f,targetPos.z }, ActionType4TimeCount / maxPushUpUpTime);

			Hand.matWorldGeneration();
		}

		if (ActionType4TimeCount == maxPushUpUpTime and isPushUpFallFlag == false)
		{
			isPushUpFallFlag = true;
		}


		//完全に止めたい時間が150
		if (waitTime > 150 and ActionType5TimeCount < maxPushUpUpReturnTime and waitTime >(PushUpReturnWaitTime + 150) and isPushUpFallFlag and isPushUpReturnFlag == false)
		{
			ActionType5TimeCount++;
		}

		//予備動作
		if (waitTime > 150 and waitTime < (PushUpReturnWaitTime + 150) and isPushUpFallFlag and isPushUpReturnFlag == false)
		{

			Hand.translation_ = { cosf(waitTime) + targetPos.x, targetPos.y + 5.1f, targetPos.z };

			Hand.matWorldGeneration();

		}

		//上に移動
		if (ActionType5TimeCount < maxPushUpUpReturnTime and isPushUpFallFlag and waitTime >(PushUpReturnWaitTime + 150) and isPushUpReturnFlag == false)
		{


			//元の位置かy座標をずらした位置に線形補間
			Hand.translation_ = lerp({ targetPos.x,targetPos.y + 11.1f,targetPos.z }, { targetPos.x,-12,targetPos.z }, ActionType5TimeCount / maxPushUpUpReturnTime);

			Hand.matWorldGeneration();
		}

		if (ActionType5TimeCount == maxPushUpUpReturnTime and isPushUpReturnFlag == false)
		{
			isPushUpReturnFlag = true;
			waitTime = 0;
		}

		//カウント
		if (returnAttackTimeCount < maxPushUpReturnTime and isPushUpReturnFlag and isPushUpReturnUpFlag == false)
		{
			returnAttackTimeCount++;
		}

		//戻る
		if (returnAttackTimeCount < maxPushUpReturnTime and isPushUpReturnFlag and isPushUpReturnUpFlag == false)
		{


			//下に移動
			Hand.translation_ = lerp({ targetPos.x,-20,targetPos.z }, { defaultPos.x,-20,defaultPos.z }, returnAttackTimeCount / maxPushUpReturnTime);

			Hand.matWorldGeneration();
		}

		if (returnAttackTimeCount == maxPushUpReturnTime and isPushUpReturnUpFlag == false)
		{
			isPushUpReturnUpFlag = true;
		}

		//カウント
		if (ActionType6TimeCount < maxPushUpDefaultUpTime and isPushUpReturnUpFlag)
		{
			ActionType6TimeCount++;
		}

		//戻る
		if (ActionType6TimeCount < maxPushUpDefaultUpTime and isPushUpReturnUpFlag)
		{


			//下に移動
			Hand.translation_ = lerp({ defaultPos.x,-20,defaultPos.z }, defaultPos, ActionType6TimeCount / maxPushUpDefaultUpTime);

			Hand.matWorldGeneration();
		}

		waitTime++;

		if (ActionType6TimeCount == maxPushUpDefaultUpTime)
		{

			isAction = false;
			isPushUpTargetMoveFlag = false;
			isPushUpUpFlag = false;
			isPushUpFallFlag = false;
			isPushUpReturnFlag = false;
			isPushUpReturnUpFlag = false;

			timeCount = 0;
			returnTimeCount = 0;
			ActionType4TimeCount = 0;
			ActionType5TimeCount = 0;
			ActionType6TimeCount = 0;
			returnAttackTimeCount = 0;

			waitTime = 0;

			isPillarPushUp = false;

		}

	}

}

void bossHand::setisActionFlag(bool flag) 
{
	isAction = flag;
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

void bossHand::setisPillarFallFlag(bool flag)
{
	isPillarFall = flag;
}

void  bossHand::setisBeamFlag(bool flag)
{
	isBeam = flag;
}


void bossHand::setisPillarRollFlag(bool flag)
{
	isPillarRoll = flag;
}

void bossHand::setisPillarPushUpFlag(bool flag)
{
	isPillarPushUp = flag;
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

void bossHand::setRotate(Vector3 rotate)
{
	
	Hand.rotation_ = rotate;

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