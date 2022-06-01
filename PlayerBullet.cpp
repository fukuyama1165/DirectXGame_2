#include "PlayerBullet.h"
#include <cassert>

PlayerBullet::PlayerBullet()
{
}

PlayerBullet::~PlayerBullet()
{
}

void PlayerBullet::Initlize(Model* model, const Vector3& position)
{
	//ヌルポチェック
	assert(model);

	model_ = model;

	textureHandle_ = TextureManager::Load("basketballman2.png");

	worldTransform_.Initialize();

	//引数で受け取った初期座標をセット
	worldTransform_.translation_ = position;


}

void PlayerBullet::Update()
{
	worldTransform_.matWorldGeneration(worldTransform_);
}

void PlayerBullet::Draw(const ViewProjection& viewProjection)
{
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}