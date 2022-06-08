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

	//�k���|�`�F�b�N
	assert(model);

	model_ = model;

	debugText_ = DebugText::GetInstance();

	//�e�N�X�`���n���h����ݒ�
	textureHandle_= TextureManager::Load("enemyBall.png");

	//���[���h�g�����X�t�H�[���̏�����
	worldTransform_.Initialize();

	//�����Ŏ󂯎�����������W���Z�b�g
	worldTransform_.translation_ = position;

	//�����Ŏ󂯎�������x���Z�b�g
	Velocity_ = velocity;

	LeaveVelocity_ = { 0,0.1f,0.1f };

}

void Enemy::Update()
{
	//���x�𑫂��Ĉړ�����
	//worldTransform_.translation_ += Velocity_;

	switch (phase_)
	{
	case Enemy::Phase::Approach:

		//�ړ�(�x�N�g�������Z)
		worldTransform_.translation_ += Velocity_;

		//�K��̈ʒu�ɓ��B�����痣�E
		if (worldTransform_.translation_.z < 0.0f)
		{
			phase_ = Phase::Leave;
		}

		break;
	case Enemy::Phase::Leave:

		//�ړ�(�x�N�g�������Z)
		worldTransform_.translation_ += LeaveVelocity_;

		break;
	default:
		break;
	}

	//�s��̍X�V
	worldTransform_.matWorldGeneration();

}

void Enemy::Draw(const ViewProjection& viewProjection)
{
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
	debugText_->SetPos(50, 90);
	debugText_->Printf("Enemypos:(%f,%f,%f)", worldTransform_.translation_.x, worldTransform_.translation_.y, worldTransform_.translation_.z);
	debugText_->SetPos(50, 110);
	switch (phase_)
	{
	case Enemy::Phase::Approach:
		debugText_->Printf("Phase:Approach");
		break;
	case Enemy::Phase::Leave:
		debugText_->Printf("Phase:Leave");
		break;
	default:
		break;
	}

}