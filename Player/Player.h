#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include "DebugText.h"
#include "Input.h"
#include "PlayerBullet.h"
#include <memory>
#include <list>

/// <summary>
/// 自キャラ
/// </summary>
class Player
{
public:
	Player();
	~Player();
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="model">モデル</param>
	/// <param name="textureHandle">テクスチャハンドル</param>
	void Initialize(Model* model, uint32_t textureHandle);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="viewProjection">ビュープロジェクション(参照渡し)</param>
	void Draw(ViewProjection& viewProjection);

	//回転(arrowキーの左右で変更)
	void Rotate();

	/// <summary>
	/// 攻撃
	/// </summary>
	void Attack();

	/// <summary>
	/// 行列とベクトルの計算(左側が行列計算の先)
	/// </summary>
	Vector3 VectorMat(Vector3 vector, Matrix4 mat);

	/// <summary>
	/// 行列とベクトルの計算(左側が行列計算の先)
	/// </summary>
	Vector3 VectorMat(Matrix4 mat, Vector3 vector);

	//ワールド座標を取得
	Vector3 GetWorldPosition();

	//衝突を検出したら呼び出されるコールバック関数
	void OnCollision();

	const std::list<std::unique_ptr<PlayerBullet>>& GetBullets() { return bullets_; };

	void SetCameraMat(Matrix4 CameraMat);

private:

	//ワールド変換データ
	WorldTransform worldTransform_;

	//モデル
	Model* model_ = nullptr;

	//テクスチャハンドル
	uint32_t textureHandle_ = 0u;

	Input* input_ = nullptr;

	DebugText* debugText_ = nullptr;

	//弾
	std::list<std::unique_ptr<PlayerBullet>> bullets_;

	Matrix4 CameraMat_ = {};

};

