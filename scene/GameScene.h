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

	std::unique_ptr<Sprite> ScoreImage;
	std::unique_ptr<Sprite> GoalImage;
	std::unique_ptr<Sprite> titleImage;

	std::unique_ptr<Sprite> SAimImage;
	std::unique_ptr<Sprite> SAttackImage;
	std::unique_ptr<Sprite> mouseImage;
	std::unique_ptr<Sprite> SMoveImage;
	std::unique_ptr<Sprite> keyImage;
	std::unique_ptr<Sprite> SOrImage;
	std::unique_ptr<Sprite> padButtonAImage;
	std::unique_ptr<Sprite> padButtonBImage;
	std::unique_ptr<Sprite> RTriggerImage;
	std::unique_ptr<Sprite> SSpaceImage;
	std::unique_ptr<Sprite> SStartImage;
	std::unique_ptr<Sprite> stickLImage;
	std::unique_ptr<Sprite> stickRImage;
	std::unique_ptr<Sprite> STitleImage;

	std::unique_ptr<Sprite> SPlayerImage;
	std::unique_ptr<Sprite> SEnemyImage;
	std::unique_ptr<Sprite> SBulletImage;
	std::unique_ptr<Sprite> SHitImage1;
	std::unique_ptr<Sprite> SHitImage2;
	std::unique_ptr<Sprite> SHitImage3;
	std::unique_ptr<Sprite> SClearTimeImage;

	uint32_t ScoreTexture = 0;
	uint32_t GoalTexture = 0;
	uint32_t titleTexture = 0;

	uint32_t SAimTexture = 0;
	uint32_t SAttackTexture = 0;
	uint32_t mouseTexture = 0;
	uint32_t SMoveTexture = 0;
	uint32_t keyTexture = 0;
	uint32_t SOrTexture = 0;
	uint32_t padButtonATexture = 0;
	uint32_t padButtonBTexture = 0;
	uint32_t RTriggerTexture = 0;
	uint32_t SSpaceTexture = 0;
	uint32_t SStartTexture = 0;
	uint32_t stickLTexture = 0;
	uint32_t stickRTexture = 0;
	uint32_t STitleTexture = 0;

	uint32_t SPlayerTexture = 0;
	uint32_t SEnemyTexture = 0;
	uint32_t SBulletTexture = 0;
	uint32_t SHitTexture = 0;
	uint32_t SclearTimeTexture = 0;

	FPS fps;

	Score clearScore[4];

	Score clear;

	float clearTimer = 0;

	int scene = 0;

	float playerHitNum = 0;
	float enemyHitNum = 0;
	float bulletHitNum = 0;

	float clearScene = 0;

	float clearPlayerHitNum = 0;
	float clearEnemyHitNum = 0;
	float clearBulletHitNum = 0;

	EffectManager effectM;
	EffectManager effectM2;

};
