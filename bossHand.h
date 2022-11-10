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

	void setisAttackFlag(bool flag);

	//プレイヤーの攻撃によって跳ね返された時１回だけ行う処理
	void playerAttackReturn();

	void punchEnd();

	void setdefaultPos(Vector3 pos) { defaultPos = pos; };

	WorldTransform GetwroldTransform() { return Hand; }

private:
	WorldTransform Hand;

	//手が攻撃中か
	bool isAttackFlag = false;
	//攻撃が終わって戻っている時か
	bool isAttackReturnFlag = false;

	//手を跳ね返した時のフラグ
	bool isReturnHand = true;

	//タイムカウンター
	float timeCount = 0;
	float returnTimeCount = 0;
	float returnAttackTimeCount = 0;

	//動作時間
	float maxTime = 500.0f;
	float maxReturnTime = 500.0f;
	float maxReturnAttackTime = 200.0f;

	//ウエイト
	float waitTime = 0;

	//待つ時間
	float attackWaitTime = 40;
	float returnWaitTime = 160;

	//プレイヤーの位置
	Vector3 targetPos = {};

	//攻撃が返された位置
	Vector3 returnPos = {};

	Vector3 defaultPos = {};

	DebugText* debugText_ = nullptr;

	Model* model;

};

