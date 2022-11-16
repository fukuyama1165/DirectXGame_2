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

	void beam();

	void setisAttackFlag(bool flag);

	void setisPressFlag(bool flag);

	void setisPressEndFlag(bool flag);

	void setisStoneFallFlag(bool flag);

	void setisBeamFlag(bool flag);
	

	//プレイヤーの攻撃によって跳ね返された時１回だけ行う処理
	void playerAttackReturn();

	void punchEnd();

	void setdefaultPos(Vector3 pos) { defaultPos = pos; };

	WorldTransform GetwroldTransform() { return Hand; }

	void setPos(Vector3 pos);

	void setScale(Vector3 scale);

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

	bool isBeamFirstStart = false;

	bool isBeamEnd = false;

	bool isGetTargetPos = false;

	//手を跳ね返した時のフラグ
	bool isReturnHand = false;

	//タイムカウンター
	float timeCount = 0;
	float returnTimeCount = 0;
	float returnAttackTimeCount = 0;
	float ActionType4TimeCount = 0;

	//動作時間
	float maxTime = 50.0f;
	float maxReturnTime = 20.0f;
	float maxReturnAttackTime = 10.0f;

	float maxSetPressTime = 20.0f;
	float maxResetPressTime = 50.0f;

	float maxTargetMoveTime = 40.0f;
	float maxFallTime = 20.0f;
	float maxFallReturnTime = 20.0f;
	float maxUpFallTime = 10.0f;

	float maxBeamTime = 100.0f;

	//ウエイト
	float waitTime = 0;

	//待つ時間
	float attackWaitTime = 40;
	float returnWaitTime = 160;

	float stoneFallWaitTime = 40;
	float stoneFallReturnWaitTime = 20;

	//今行動しているか
	bool isAction = false;
	bool isStoneFallAction = false;

	bool isPress = false;
	bool isPressEnd = true;
	bool isStoneFall = false;
	bool isBeam = false;

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

