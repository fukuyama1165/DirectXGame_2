#include "Enemy.h"
#include "EnemyStateApproach.h"
#include <cassert>

Enemy::Enemy()
{
}

Enemy::~Enemy()
{
	delete state_;
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

	//���E���鎞�̃X�s�[�h���Z�b�g
	LeaveVelocity_ = { 0,0.1f,0.1f };

	//�������߂Â��̂ŏ���������
	state_ = new EnemyStateApproach(getThis());
	
	//�e�̔��˂��X�^�[�g����
	FireAndReset();

}

void Enemy::Update()
{
	//���݂�state�ōX�V����
	state_->Update();

	//
	//(this->*PhaseMoveP[static_cast<size_t>(phase_)])();

	//�s��̍X�V
	worldTransform_.matWorldGeneration();

	//�f�X�t���O�̗������e���폜(remove_if->������v��S�č폜)
	bullets_.remove_if([](std::unique_ptr<EnemyBullet>& bullet)//if�̒��ŊȈՓI�Ȋ֐��𐶐����Ă�->[](����)
	{
		return bullet->IsDead();
	});

	//if (FireTime_ == 0)
	//{
	//	//���ˊ֐�
	//	Fire();

	//	FireTime_ = kFireInterval;
	//}

	//�J�E���g�_�E�����I�����Ă��������
	timedCalls_.remove_if([](std::unique_ptr<TimeCall>& timeCall)
	{
		return timeCall->IsFinished();
	});

	//��������Ă���J�E���g�_�E�����X�V
	for (std::unique_ptr<TimeCall>& timeCall : timedCalls_)
	{
		timeCall->Updata();
	}

	//�e�̍X�V����
	for (std::unique_ptr<EnemyBullet>& bullet : bullets_)
	{
		bullet->Update();
	}

	

}

void Enemy::Draw(const ViewProjection& viewProjection)
{
	//���f����`��
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
	debugText_->SetPos(50, 90);
	debugText_->Printf("Enemypos:(%f,%f,%f)", worldTransform_.translation_.x, worldTransform_.translation_.y, worldTransform_.translation_.z);
	
	
	//�������ꂽ�e��`��
	for (std::unique_ptr<EnemyBullet>& bullet : bullets_)
	{
		bullet->Draw(viewProjection);
	}

}

void Enemy::ApproachMove()
{
	//�ړ�(�x�N�g�������Z)
	worldTransform_.translation_ += Velocity_;

	//�K��̈ʒu�ɓ��B�����痣�E
	if (worldTransform_.translation_.z < 0.0f)
	{
		phase_ = Phase::Leave;
	}
}

void Enemy::LeaveMove()
{
	//�ړ�(�x�N�g�������Z)
	worldTransform_.translation_ += LeaveVelocity_;

}

//�֐��|�C���^���Z�b�g
void (Enemy::* Enemy::PhaseMoveP[])() =
{
	&Enemy::ApproachMove,
	&Enemy::LeaveMove
};

void Enemy::MoveTranslation(Vector3 Velocity)
{
	worldTransform_.translation_ += Velocity;
}

Vector3	Enemy::GetPos()
{
	return worldTransform_.translation_;
}

void Enemy::ChangeState(BaseEnemyState* newState)
{
	delete state_;
	state_ = newState;
}

Enemy* Enemy::getThis()
{
	return this;
}

void Enemy::Fire()
{
	//���˒n�_�ׂ̈ɃL�����̍��W���R�s�[
	Vector3 position = worldTransform_.translation_;

	//�ړ��ʂ�ǉ�
	const float kBulletSpeed = -2.0f;
	Vector3 velocity(0, 0, kBulletSpeed);


	//�e�̐����Ə�����
	std::unique_ptr<EnemyBullet> newBullet = std::make_unique<EnemyBullet>();
	newBullet->Initlize(model_, position, velocity);

	//�e��o�^
	bullets_.push_back(std::move(newBullet));
}


void Enemy::SetFireTime(int32_t FireTime)
{
	FireTime_ = FireTime;
}

int32_t Enemy::GetFireTime()
{
	return FireTime_;
}

void Enemy::FireAndReset()
{
	//�e�𔭎˂���
	Fire();

	//���˃^�C�}�[���Z�b�g����
	timedCalls_.push_back(std::make_unique<TimeCall>(std::bind(&Enemy::FireAndReset, this), kFireInterval));

}

void Enemy::FireTimeReMoved()
{
	//�����I�ɑS�Ă̗v�f������
	timedCalls_.remove_if([](std::unique_ptr<TimeCall>& timeCall)
	{
		return 1;
	});
}
