#include "Enemy.h"
#include <cassert>

Enemy::Enemy()
{
}

Enemy::~Enemy()
{
}

void Enemy::Initialize(Model* model, const Vector3& position, const Vector3& velocity)
{

	//ヌルポチェック
	assert(model);

	model_ = model;

	debugText_ = DebugText::GetInstance();

	//テクスチャハンドルを設定
	textureHandle_= TextureManager::Load("enemyBall.png");

	//ワールドトランスフォームの初期化
	worldTransform_.Initialize();

	//引数で受け取った初期座標をセット
	worldTransform_.translation_ = position;

	//引数で受け取った速度をセット
	Velocity_ = velocity;

}

void Enemy::Update()
{
	//速度を足して移動する
	worldTransform_.translation_ += Velocity_;


	//行列の更新
	worldTransform_.matWorldGeneration();

}

void Enemy::Draw(const ViewProjection& viewProjection)
{
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
	debugText_->SetPos(50, 90);
	debugText_->Printf("Enemypos:(%f,%f,%f)", worldTransform_.translation_.x, worldTransform_.translation_.y, worldTransform_.translation_.z);
}