#include "Enemy.h"
#include "EnemyStateApproach.h"
#include "Player.h"
#include "GameScene.h"
#include <cassert>

Enemy::Enemy()
{
}

Enemy::~Enemy()
{
	delete state_;
}

void Enemy::Initialize(Model* model, Model* enemyModel, const Vector3& position, const Vector3& velocity)
{

	//�k���|�`�F�b�N
	assert(model);
	assert(enemyModel);

	model_ = model;
	enemyModel_ = enemyModel;

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
	state_->Update(Velocity_);

	//
	//(this->*PhaseMoveP[static_cast<size_t>(phase_)])();

	//�s��̍X�V
	worldTransform_.matWorldGeneration();

	if (worldTransform_.matWorldGetPos().y > 80)
	{
		IsDead_ = true;
	}

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

	

}

void Enemy::Draw(const ViewProjection& viewProjection)
{
	//���f����`��
	enemyModel_->Draw(worldTransform_, viewProjection);
	/*debugText_->SetPos(50, 90);
	debugText_->Printf("Enemypos:(%f,%f,%f)", worldTransform_.translation_.x, worldTransform_.translation_.y, worldTransform_.translation_.z);*/
	
	
	

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
	////���˒n�_�ׂ̈ɃL�����̍��W���R�s�[
	//Vector3 position = worldTransform_.translation_;

	////�ړ��ʂ�ǉ�
	//const float kBulletSpeed = -2.0f;
	//Vector3 velocity(0, 0, kBulletSpeed);

	assert(player_);

	//�e�̑��x
	const float kBulletSpeed = 1.0f;

	Vector3 playerVec;
	Vector3 enemyVec;
	Vector3 enemyPlayerVec;

	playerVec = player_->GetWorldPosition();
	enemyVec = GetWorldPosition();

	enemyPlayerVec = { playerVec.x - enemyVec.x,playerVec.y - enemyVec.y,playerVec.z - enemyVec.z };

	float len = sqrtf(enemyPlayerVec.x * enemyPlayerVec.x + enemyPlayerVec.y * enemyPlayerVec.y + enemyPlayerVec.z * enemyPlayerVec.z);

	enemyPlayerVec /= len;

	Vector3 bullrtSpeed = { enemyPlayerVec.x * kBulletSpeed,enemyPlayerVec.y * kBulletSpeed,enemyPlayerVec.z * kBulletSpeed };

	//�e�̐����Ə�����
	std::unique_ptr<EnemyBullet> newBullet = std::make_unique<EnemyBullet>();
	newBullet->Initlize(model_, worldTransform_.translation_, bullrtSpeed);

	//�e��o�^
	//bullets_.push_back(std::move(newBullet));

	gameScene_->AddEnemyBullet(std::move(newBullet));

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

Vector3 Enemy::GetWorldPosition()
{
	//���[���h���W������ϐ�
	Vector3 worldpos;

	//���[���h�s��̕��s�ړ��������擾(���[���h���W)
	worldpos.x = worldTransform_.matWorld_.m[3][0];
	worldpos.y = worldTransform_.matWorld_.m[3][1];
	worldpos.z = worldTransform_.matWorld_.m[3][2];

	return worldpos;
}

void Enemy::OnCollision()
{
	HP--;

	if (HP <= 0)
	{
		IsDead_ = true;
	}
}
