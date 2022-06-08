#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include "DebugText.h"

/// <summary>
/// 敵
/// </summary>
class Enemy
{
public:
	Enemy();
	~Enemy();

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="model">モデル</param>
	/// <param name="position">初期座標</param>
	/// /// <param name="velocity">速度</param>
	void Initialize(Model* model, const Vector3& position, const Vector3& velocity);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="viewProjection">ビュープロジェクション</param>
	void Draw(const ViewProjection& viewProjection);


	//接近するときの動作をする関数
	void ApproachMove();

	//離脱するときの動作をする関数
	void LeaveMove();

private:

	//メンバ関数ポインタ
	static void (Enemy::*PhaseMoveP[])();

private:

	//ワールドトランスフォーム
	WorldTransform worldTransform_;

	//テクスチャハンドル
	uint32_t textureHandle_ = 0;

	//3Dモデル
	Model* model_ = nullptr;

	//速度
	Vector3	Velocity_;

	DebugText* debugText_ = nullptr;

	enum class Phase {
		Approach,//接近する
		Leave,//離脱する
	};

	//敵の挙動について
	Phase phase_ = Phase::Approach;

	//離脱するときのスピード
	Vector3 LeaveVelocity_;

	

};


