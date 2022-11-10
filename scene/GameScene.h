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
#include "Skydome.h"
#include "RailCamera.h"
#include "Score.h"
#include "FPS.h"

#include <sstream>

#include <memory>

#include "EffectManager.h"

#include "bosstest.h"

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

	/// <summary>
	/// 敵弾を追加する
	/// </summary>
	/// <param name="enemyBullet">敵弾</param>
	void AddEnemyBullet(std::unique_ptr<EnemyBullet> enemyBullet);

	const std::list<std::unique_ptr<EnemyBullet>>& GetEnemyBullets() { return enemyBullets_; };

	/// <summary>
	/// 敵発生データの読み込み
	/// </summary>
	void LoadEnemyPopData();

	/// <summary>
	/// 敵発生コマンドの更新
	/// </summary>
	void UpdateEnemyPopCommands();

	void PopEnemy(Vector3 pos);

	void resetGame();

private:
	//二次元のアフィン変換
	void afin(WorldTransform Transform);

	void AllCol();

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
	Model* playerModel_ = nullptr;
	Model* enemyModel_ = nullptr;
	Model* playerbulletModel_ = nullptr;
	Model* enemybulletModel_ = nullptr;

	//ワールドトランスフォーム
	WorldTransform worldTransform_;

	WorldTransform title;

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
	//Enemy* enemy_p = nullptr;

	std::list<std::unique_ptr<Enemy>> enemy_;

	//天球
	Skydome* skydome_p = nullptr;

	std::unique_ptr<Skydome> skydome_;

	//3Dモデル
	Model* modelSkydome_ = nullptr;

	//レールカメラ
	RailCamera* railCamera_p = nullptr;

	std::unique_ptr<RailCamera> railCamera_;

	float cameraSpeed = 0;

	ViewProjection railView;
	ViewProjection titleView;

	//敵弾
	std::list<std::unique_ptr<EnemyBullet>> enemyBullets_;

	//敵発生コマンド
	std::stringstream enemyPopCommands;
	std::stringstream ReEnemyPopCommands;

	bool isWait = false;

	int32_t waitTimer = 0;

	Score scoore;
	Score Goal;

	uint32_t textureNum_[10] = {};


	FPS fps;

	
	
	
	
	
	
	float rotateX=0;
	float rotateY=0;

	float cameraRotateX = 0;
	float cameraRotateY = 9.45f;

	bosstest boss;

};
