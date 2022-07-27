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
#include "Player.h"
#include "Enemy.h"

#include <memory>


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

	/// <summary>
	/// 衝突判定と応答
	/// </summary>
	void CheckAllCollision();

private:
	//二次元のアフィン変換
	void afin(WorldTransform Transform);

private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;
	DebugText* debugText_ = nullptr;

	/// <summary>
	/// ゲームシーン用
	/// </summary>


	//自キャラ
	std::unique_ptr<Player> player_;

	Player* player_p = nullptr;

	//テクスチャハンドル
	uint32_t textureHandle_ = 0;

	//3Dモデル
	Model* model_ = nullptr;

	//ワールドトランスフォーム
	//WorldTransform worldTransform_;


	//ビュープロジェクション
	ViewProjection viewProjection_;

	//カメラ上方向の角度
	float viewAngle = 0.0f;

	//デバックカメラ
	DebugCamera* debugCamera_ = nullptr;

	bool IsdebugCameraUse = false;

	Vector3 siten;
	Vector3 syuten;
	Vector4 color;

	float move = 0;

	float Cameramove = 0;

	//敵
	Enemy* enemy_p = nullptr;

	std::unique_ptr<Enemy> enemy_;



};
