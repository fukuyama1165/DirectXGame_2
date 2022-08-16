#include "EnemyBullet.h"
#include <cassert>

EnemyBullet::EnemyBullet()
{
}

EnemyBullet::~EnemyBullet()
{
}

void EnemyBullet::Initlize(Model* model, const Vector3& position, const Vector3& velocity)
{
	//ヌルポチェック
	assert(model);

	model_ = model;

	textureHandle_ = TextureManager::Load("enemyBall.png");

	//引数で受け取った初期座標をセット
	worldTransform_.translation_ = position;

	Velocity_ = velocity;

	worldTransform_.scale_.x = 0.5f;
	worldTransform_.scale_.y = 0.5f;
	worldTransform_.scale_.z = 3.0f;
	worldTransform_.Initialize();

	//Y軸周り角度(Θy)
	worldTransform_.rotation_.y = std::atan2(velocity.x,velocity.z);

	worldTransform_.rotation_.y = worldTransform_.rotation_.y;

	Matrix4 rotMat = worldTransform_.matRotateGeneration();
	
	Vector3 velocityZ = MathUtility::operator*(Velocity_, rotMat);

	//X軸周り角度(Θx)
	worldTransform_.rotation_.x = std::atan2(-velocityZ.y, velocityZ.z);

}

void EnemyBullet::Update()
{
	//移動するところ
	worldTransform_.translation_ += Velocity_;
	worldTransform_.matWorldGeneration();

	//デスタイマーをひいて0以下になったらフラグを立てる
	if (--deathTimer_ <= 0)
	{
		isDead_ = true;
	}

}

void EnemyBullet::Draw(const ViewProjection& viewProjection)
{
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}