#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include "DebugText.h"

class bossHand
{
public:

	void init(Vector3 scale, Vector3 rotate, Vector3 translation,Model* model);

	void update(WorldTransform worldTransform);

	void draw(const ViewProjection& viewProjection);

	bool getisAttackFlag() { return isAttackFlag; };

	bool getisReturnHand() { return isReturnHand; };
	
	bool getisPressEndd() { return isPressEnd; };

	bool getisAction() { return isAction; };

	bool getisFallTargetMoveFlag() { return isFallTargetMoveFlag; };

	bool getisGetTargetPosFlag() { return isGetTargetPos; };

	void punch(WorldTransform worldTransform);

	void press();

	void stoneFall();
	void pillarFall();

	void beam();


	void pillarRoll();

	void pillarPushUp();

	//これでフラグをtrueにし続けると挙動がおかしくなるので注意(使ったら戻すこと)
	void setisActionFlag(bool flag);

	void setisAttackFlag(bool flag);

	void setisPressFlag(bool flag);

	void setisPressEndFlag(bool flag);

	void setisStoneFallFlag(bool flag);

	void setisPillarFallFlag(bool flag);

	void setisBeamFlag(bool flag);
	
	void setisPillarRollFlag(bool flag);

	void setisPillarPushUpFlag(bool flag);

	//プレイヤーの攻撃によって跳ね返された時１回だけ行う処理
	void playerAttackReturn();

	void punchEnd();

	void setdefaultPos(Vector3 pos) { defaultPos = pos; };

	WorldTransform GetwroldTransform() { return Hand; }

	void setPos(Vector3 pos);

	void setScale(Vector3 scale);

	void setRotate(Vector3 rotate);

	void setTargetPos(Vector3 target);

private:
	WorldTransform Hand;

	//手が攻撃中か
	bool isAttackFlag = false;
	//攻撃が終わって戻っている時か
	bool isAttackReturnFlag = false;

	bool isFallTargetMoveFlag = false;
	bool isFallFallFlag = false;
	bool isFallReturnFlag = false;

	bool isPillarFallReturnFlag = false;

	bool isBeamFirstStart = false;

	bool isBeamEnd = false;

	bool isPillarRollFirstStart = false;
	

	bool isPushUpTargetMoveFlag = false;
	bool isPushUpUpFlag = false;
	bool isPushUpFallFlag = false;
	bool isPushUpReturnFlag = false;
	bool isPushUpReturnUpFlag = false;

	bool isGetTargetPos = false;

	//手を跳ね返した時のフラグ
	bool isReturnHand = false;

	//タイムカウンター
	float timeCount = 0;
	float returnTimeCount = 0;
	float returnAttackTimeCount = 0;
	float ActionType4TimeCount = 0;
	float ActionType5TimeCount = 0;
	float ActionType6TimeCount = 0;

	//動作時間
	float maxTime = 50.0f;
	float maxReturnTime = 20.0f;
	float maxReturnAttackTime = 10.0f;

	//プレス
	float maxSetPressTime = 20.0f;
	float maxResetPressTime = 50.0f;

	//石落とし
	float maxTargetMoveTime = 35.0f;
	float maxFallTime = 10.0f;
	float maxFallReturnTime = 10.0f;
	float maxUpFallTime = 10.0f;

	//第二段階用石落とし
	float maxUpPillarFallTime = 20.0f;

	//ビーム
	float maxBeamTime = 30.0f;
	float maxFirstBeamTime = 20.0f;

	//回転
	float maxPillarRollTime = 30.0f;

	//突き上げ
	float maxPushUpFallTime = 10.0f;
	float maxPushUpTargetMoveTime = 35.0f;
	float maxPushUpUpTime = 10.0f;
	float maxPushUpUpReturnTime = 20.0f;
	float maxPushUpReturnTime = 10.0f;
	float maxPushUpDefaultUpTime = 20.0f;

	//ウエイト
	float waitTime = 0;

	//待つ時間
	float attackWaitTime = 40;
	float returnWaitTime = 160;

	float stoneFallWaitTime = 40;
	float stoneFallReturnWaitTime = 10;

	float PushUpWaitTime = 40;
	float PushUpReturnWaitTime = 10;

	//今行動しているか
	bool isAction = false;
	bool isStoneFallAction = false;

	//
	bool isPress = false;
	bool isPressEnd = true;
	bool isStoneFall = false;
	bool isPillarFall = false;
	bool isPillarRoll = false;
	bool isBeam = false;
	bool isPillarPushUp = false;

	//プレイヤーの位置
	Vector3 targetPos = {};

	//攻撃が返された位置
	Vector3 returnPos = {};

	Vector3 defaultPos = {};

	Vector3 startBeamScale = { 5,5,5 };

	DebugText* debugText_ = nullptr;

	Model* model;

};

const Vector3 lerp(const Vector3& start, const Vector3& end, const float t);

