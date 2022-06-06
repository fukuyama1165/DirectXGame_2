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
	//�k���|�`�F�b�N
	assert(model);

	model_ = model;

	textureHandle_ = TextureManager::Load("basketballman2.png");

	worldTransform_.Initialize();

	//�����Ŏ󂯎�����������W���Z�b�g
	worldTransform_.translation_ = position;

	Velocity_ = velocity;


}

void PlayerBullet::Update()
{
	//�ړ�����Ƃ���
	worldTransform_.translation_ += Velocity_;
	worldTransform_.matWorldGeneration();

	//�f�X�^�C�}�[���Ђ���0�ȉ��ɂȂ�����t���O�𗧂Ă�
	if (--deathTimer_ <= 0)
	{
		isDead_ = true;
	}

}

void PlayerBullet::Draw(const ViewProjection& viewProjection)
{
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}