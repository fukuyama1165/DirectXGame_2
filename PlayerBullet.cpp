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
	//�k���|�`�F�b�N
	assert(model);

	model_ = model;

	textureHandle_ = TextureManager::Load("basketballman2.png");

	worldTransform_.Initialize();

	//�����Ŏ󂯎�����������W���Z�b�g
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