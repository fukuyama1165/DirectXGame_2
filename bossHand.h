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

	void punch(WorldTransform worldTransform);

	void press(WorldTransform worldTransform);

	void setisAttackFlag(bool flag);

	void setisPressFlag(bool flag);

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

	//手を跳ね返した時のフラグ
	bool isReturnHand = false;

	//タイムカウンター
	float timeCount = 0;
	float returnTimeCount = 0;
	float returnAttackTimeCount = 0;

	//動作時間
	float maxTime = 500.0f;
	float maxReturnTime = 500.0f;
	float maxReturnAttackTime = 200.0f;

	float maxSetPressTime = 20.0f;

	//ウエイト
	float waitTime = 0;

	//待つ時間
	float attackWaitTime = 40;
	float returnWaitTime = 160;

	//今行動しているか
	bool isAction = false;

	bool isPress = false;

	//プレイヤーの位置
	Vector3 targetPos = {};

	//攻撃が返された位置
	Vector3 returnPos = {};

	Vector3 defaultPos = {};

	DebugText* debugText_ = nullptr;

	Model* model;

};

const Vector3 lerp(const Vector3& start, const Vector3& end, const float t);

