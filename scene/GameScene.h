#pragma once

#include "Audio.h"
#include "DirectXCommon.h"
#include "DebugText.h"
#include "Input.h"
#include "Model.h"
#include "SafeDelete.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "DebugCamera.h"

/// <summary>
/// ゲームシーン
/// </summary>
class GameScene {

public: // メンバ関数
  /// <summary>
  /// コンストクラタ
  /// </summary>
	GameScene();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameScene();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();



private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;
	DebugText* debugText_ = nullptr;

	/// <summary>
	/// ゲームシーン用
	/// </summary>

	//テクスチャハンドル
	uint32_t textureHandle_ = 0;

	//3Dモデル
	Model* model_ = nullptr;

	//ワールドトランスフォーム
	WorldTransform worldTransforms_[100];

	//ビュープロジェクション
	ViewProjection viewProjection_;

	//カメラ上方向の角度
	float viewAngle = 0.0f;
	
	//デバックカメラ
	DebugCamera* debugCamera_ = nullptr;

	Vector3 siten;
	Vector3 syuten;
	Vector4 color;

	float move = 0;

	void afin(WorldTransform Transform);

	Matrix4 matScaleGeneration(Vector3 scale);
	Matrix4 matRotateXGeneration(float rotateX);
	Matrix4 matRotateYGeneration(float rotateY);
	Matrix4 matRotateZGeneration(float rotateZ);
	Matrix4 matRotateGeneration(Vector3 rotate);
	Matrix4 matMoveGeneration(Vector3 move);
	void matWorldGeneration(WorldTransform& worldTransform);

	//親持ちのworldTransformの移動行列関数
	void matWorldParentGeneration(WorldTransform& worldTransform);

	enum PartId
	{
		kRoot,//大元
		kSpine,//脊髄
		kChest,//胸
		kHead,//頭
		kArmL,//左腕
		kArmR,//右腕
		kHip,//尻
		kLegL,//左足
		kLegR,//右足

		kNumPartId
	};

	float RadianChange(float Angle);
	float AngleChange(float Radian);

};
