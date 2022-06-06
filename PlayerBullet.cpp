#include "PlayerBullet.h"
#include <cassert>

PlayerBullet::PlayerBullet()
{
}

PlayerBullet::~PlayerBullet()
{
}

void PlayerBullet::Initlize(Model* model, const Vector3& position, const Vector3& velocity)
{
	//ヌルポチェック
	assert(model);

	model_ = model;

	textureHandle_ = TextureManager::Load("basketballman2.png");

	worldTransform_.Initialize();

	//引数で受け取った初期座標をセット
	worldTransform_.translation_ = position;

	Velocity_ = velocity;


}

void PlayerBullet::Update()
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

void PlayerBullet::Draw(const ViewProjection& viewProjection)
{
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}