#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include "DebugText.h"
#include "BaseEnemyState.h"
#include "EnemyBullet.h"
#include <memory>
#include <list>
#include "TimeCall.h"

class BaseEnemyState;
class Player;

class GameScene;

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
	void Initialize(Model* model, Model* enemyModel, const Vector3& position, const Vector3& velocity);

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

	//velocity分Translation移動させる関数
	void MoveTranslation(Vector3 Velocity);

	//現在位置を返す関数
	Vector3 GetPos();
	
	//スタイル変更用関数
	void ChangeState(BaseEnemyState* newStaete);

	//自分を返す関数
	Enemy* getThis();

	//弾発射
	void Fire();

	//fireTimeをセットする関数
	void SetFireTime(int32_t FireTime);

	//fireTimeを取得する関数
	int32_t GetFireTime();

	//弾を発射してリセットする(firetimeの間隔を設定し直す)
	void FireAndReset();

	//強制的に弾を発射できないようにする
	void FireTimeReMoved();

	void SetPlayer(Player* player) { player_ = player; };

	void SetGameScene(GameScene* gameScene) { gameScene_ = gameScene; };

	//ワールド座標を取得
	Vector3 GetWorldPosition();

	//衝突を検出したら呼び出されるコールバック関数
	void OnCollision();

	bool IsDead()const { return IsDead_; }
	

private:

	//メンバ関数ポインタ
	static void (Enemy::*PhaseMoveP[])();


public:

	//発射間隔設定用
	static const int kFireInterval = 50;

private:

	//ワールドトランスフォーム
	WorldTransform worldTransform_;

	//テクスチャハンドル
	uint32_t textureHandle_ = 0;

	//3Dモデル
	Model* model_ = nullptr;
	Model* enemyModel_ = nullptr;

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

	//行動用のポインタ
	BaseEnemyState *state_=nullptr;

	

	//発射間隔
	int32_t FireTime_ = 0;

	//カウントダウンのリスト
	std::list<std::unique_ptr<TimeCall>> timedCalls_;

	Player* player_ = nullptr;

	GameScene* gameScene_ = nullptr;

	int HP = 3;

	bool IsDead_ = false;

};


