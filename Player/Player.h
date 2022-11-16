#pragma once
#include "WinApp.h"
#include "MathUtility.h"
#include "Model.h"
#include "WorldTransform.h"
#include "DebugText.h"
#include "Input.h"
#include "PlayerBullet.h"
#include <memory>
#include <list>
#include"RailCamera.h"
#include"bosstest.h"

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
	void Initialize(RailCamera* camera,bosstest* boss);
	void ResourceInitialize(Model* model, Model* playerModel, uint32_t textureHandle, uint32_t Reticletexture);

	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="viewProjection">ビュープロジェクション(参照渡し)</param>
	void Update(ViewProjection viewProjection);

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="viewProjection">ビュープロジェクション(参照渡し)</param>
	void Draw(ViewProjection& viewProjection);

	void DrawUI();
	
	Vector2 kasu(WorldTransform obj);

	/// <summary>
	/// UI描画
	
	/// <summary>
	/// 攻撃
	/// </summary>
	void Attack(Vector3 flont);

	/// <summary>
	/// 行列とベクトルの計算(左側が行列計算の先)
	/// </summary>
	Vector3 VectorMat(Vector3 vector, Matrix4 mat);

	/// <summary>
	/// 行列とベクトルの計算(左側が行列計算の先)
	/// </summary>
	Vector3 VectorMat(Matrix4 mat, Vector3 vector);

	Vector3 PlayerMoveVec();

	//ワールド座標を取得
	Vector3 GetWorldPosition();

	WorldTransform GetMat();

	Vector2 poskure() { return bosstarget->GetPosition(); }

	bool LockOn(WorldTransform obj);

	void SetWorldPosition(Vector3 osimodosi);

	//衝突を検出したら呼び出されるコールバック関数
	void OnCollision();

	const std::list<std::unique_ptr<PlayerBullet>>& GetBullets() { return bullets_; };


	void reset();

private:

	RailCamera* camera = nullptr;

	bosstest* boss = nullptr;

	

	//ワールド変換データ
	WorldTransform worldTransform_;
	WorldTransform target;
	WorldTransform target2;
	WorldTransform target3;
	WorldTransform target4;
	WorldTransform target5;
	

	ViewProjection viewProjection_;

	//モデル
	Model* model_ = nullptr;
	Model* playerModel_ = nullptr;

	//テクスチャハンドル
	uint32_t textureHandle_ = 0u;
	uint32_t ReticleTexture = 0u;

	Input* input_ = nullptr;

	DebugText* debugText_ = nullptr;

	//弾
	std::list<std::unique_ptr<PlayerBullet>> bullets_;

	std::unique_ptr<Sprite> Reticle;
	std::unique_ptr<Sprite> bosstarget;

	int bulletCT = 0;

	const int MAXBulletCT = 5;

	Vector3 moveVec;

	float move_speed;

	bool hopper_dash;

	int cooltime;

	int yuyotime;

	int hopper_count;

	const int hopper_limit = 4;

	float hopper_speed;

	bool cooldown;

	Vector3 dash_vec;

	bool B_bottan;
	bool old_B_bottan;

	const int firelate = 15;

	int latetime;

	int width;
	int height;



};

