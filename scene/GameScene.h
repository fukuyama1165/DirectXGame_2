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
	WorldTransform worldTransform_;

	//ビュープロジェクション
	ViewProjection viewProjection_[4];

	//カメラ上方向の角度
	float viewAngle = 0.0f;

	//デバックカメラ
	DebugCamera* debugCamera_ = nullptr;

	Vector3 siten;
	Vector3 syuten;
	Vector4 color;

	float move = 0;

	float viewCamera = 0;

	void afin(WorldTransform Transform);

	Matrix4 matScaleGeneration(Vector3 scale);
	Matrix4 matRotateXGeneration(float rotateX);
	Matrix4 matRotateYGeneration(float rotateY);
	Matrix4 matRotateZGeneration(float rotateZ);
	Matrix4 matRotateGeneration(Vector3 rotate);
	Matrix4 matMoveGeneration(Vector3 move);
	void matWorldGeneration(WorldTransform worldTransform);

};
