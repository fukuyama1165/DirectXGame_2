#pragma once
#include "WorldTransform.h"
#include "Model.h"
#include "Vector3.h"

class Effect
{
public:
	Effect();
	~Effect();

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="pos">生成場所</param>
	/// <param name="moveVec">正規化した移動ベクトル</param>
	/// <param name="LiveTime">画面に存在する時間</param>
	void Init(Model* model,Vector3 pos, Vector3 moveVec, float LiveTime = 50);

	/// <summary>
	/// 更新
	/// </summary>
	
	void Update();
	void Draw(ViewProjection& viewProjection);

	void SetPos(Vector3 pos);
	void SetSpeed(float speed);

	bool GetIsDead() { return isDead; };

private:
	//モデル
	Model* model_ = nullptr;

	WorldTransform worldTransform;
	Vector3 moveVector;
	bool isDead = false;
	float liveTime = 50;
	float moveSpeed = 1;

};