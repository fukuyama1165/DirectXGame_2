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
	//�k���|�`�F�b�N
	assert(model);

	model_ = model;

	textureHandle_ = TextureManager::Load("hokehoke.png");

	worldTransform_.Initialize();

	//�����Ŏ󂯎�����������W���Z�b�g
	worldTransform_.translation_ = position;

	Velocity_ = velocity;


}

void EnemyBullet::Update()
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

void EnemyBullet::Draw(const ViewProjection& viewProjection)
{
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}