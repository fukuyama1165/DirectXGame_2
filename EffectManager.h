#pragma once
#include "Effect.h"
#include <vector>
#include "Vector3.h"
#include "Model.h"
#include "WorldTransform.h"

class EffectManager
{
public:
	EffectManager();
	~EffectManager();

	void Init();
	void Update();

	void ExplosionEffect(int activeTime);
	void popEffect(Vector3 pos, float liveTime = 50);

	void setPos(Vector3 pos);

	void draw(ViewProjection& viewProjection);

	WorldTransform getWorldTransform();

	bool GetIsEffctEnd() { return isEffectEnd; };


private:

	std::vector<std::unique_ptr<Effect>> effects;

	//実行時間が終わったらtrueになるFlag
	bool isEffectEnd = false;

	WorldTransform worldTransform;

	Model* model = nullptr;

	//単体がどっちにに動くかを入れる変数
	Vector3 moveVecor = {};

	//始まったか(タイマーセットのため)
	bool isStart = false;
	//エフェクトを動かす時間
	int activeTimer = 0;

	
	const int maxCoolTime = 3;

	int coolTime = maxCoolTime;

	float effctSpeed = 0.5f;

};

int GetRand(int min, int max);
float GetRand(float maxRand);
