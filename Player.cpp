#include "Player.h"
#include <cassert>

void Player::Initialize(Model* model, uint32_t textureHandle)
{
	//NULL�|�C���^�`�F�b�N
	assert(model);

	model_ = model;
	textureHandle_ = textureHandle;

	//�V���O���g���C���X�^���X���擾����
	input_ = Input::GetInstance();
	debugText_ = DebugText::GetInstance();

	//���[���h�ϊ��̏�����
	worldTransform_.Initialize();
}

Player::Player()
{
}

Player::~Player()
{
}

void Player::Update()
{

	//�f�X�t���O�̗������e���폜(remove_if->������v��S�č폜)
	bullets_.remove_if([](std::unique_ptr<PlayerBullet>& bullet)//if�̒��ŊȈՓI�Ȋ֐��𐶐����Ă�->[](����)
	{
		return bullet->IsDead();
	});

	//�L�����N�^�[�̈ړ��x�N�g��
	Vector3 move = { 0,0,0 };

	const float MoveSpeed = 1;

	if (input_->PushKey(DIK_W))
	{
		move = { 0,MoveSpeed,0 };
	}

	if (input_->PushKey(DIK_S))
	{
		move = { 0,-MoveSpeed,0 };
	}

	if (input_->PushKey(DIK_D))
	{
		move = { MoveSpeed,0,0 };
	}

	if (input_->PushKey(DIK_A))
	{
		move = { -MoveSpeed,0,0 };
	}

	//�ړ����E���W
	const float kMoveLimitX = 30;
	const float kMoveLimitY = 15;

	worldTransform_.translation_ += move;

	worldTransform_.translation_.x = max(worldTransform_.translation_.x, -kMoveLimitX);
	worldTransform_.translation_.x = min(worldTransform_.translation_.x, +kMoveLimitX);

	worldTransform_.translation_.y = max(worldTransform_.translation_.y, -kMoveLimitY);
	worldTransform_.translation_.y = min(worldTransform_.translation_.y, +kMoveLimitY);

	//���W�ړ�(�s��X�V�����̊֐���)
	worldTransform_.matWorldGeneration();

	//��]����arrow�L�[�̍��E�ŉ�]������
	Rotate();

	//Space�L�[�Œe�𐶐�����player�̐��ʂɐi�܂���
	Attack();

	for (std::unique_ptr<PlayerBullet>& bullet : bullets_)
	{
		bullet->Update();
	}

	debugText_->SetPos(50, 70);
	debugText_->Printf("pos:(%f,%f,%f)", worldTransform_.translation_.x, worldTransform_.translation_.y, worldTransform_.translation_.z);

}

void Player::Draw(ViewProjection& viewProjection)
{

	//3D���f����`��
	model_->Draw(worldTransform_, viewProjection, textureHandle_);

	for(std::unique_ptr<PlayerBullet>& bullet:bullets_)
	{
		bullet->Draw(viewProjection);
	}

}

void Player::Rotate()
{
	//�L�����N�^�[�̈ړ��x�N�g��
	Vector3 rotate = { 0,0,0 };

	const float MoveSpeed = 0.1;

	if (input_->PushKey(DIK_RIGHT))
	{
		rotate = { 0,MoveSpeed,0 };
	}

	if (input_->PushKey(DIK_LEFT))
	{
		rotate = { 0,-MoveSpeed,0 };
	}

	worldTransform_.rotation_ += rotate;

	//���W�ړ�(�s��X�V�����̊֐���)
	worldTransform_.matWorldGeneration();
}

void Player::Attack()
{
	if (input_->TriggerKey(DIK_SPACE))
	{
		//���˒n�_�ׂ̈Ɏ��L�����̍��W���R�s�[
		Vector3 position = worldTransform_.translation_;

		//�ړ��ʂ�ǉ�
		const float kBulletSpeed = 1.0f;
		Vector3 velocity(0, 0, kBulletSpeed);

		//���x�x�N�g�������@�̌����ɍ��킹�ĉ�]����
		velocity = VectorMat(velocity, worldTransform_.matWorld_);

		//�e�̐����Ə�����
		std::unique_ptr<PlayerBullet> newBullet = std::make_unique<PlayerBullet>();
		newBullet->Initlize(model_, position,velocity);

		//�e��o�^
		bullets_.push_back(std::move(newBullet));
	}
}

Vector3 Player::VectorMat(Vector3 vector, Matrix4 mat)
{
	Vector3 changeVector = { 0,0,0 };

	changeVector.x = vector.x * mat.m[0][0] + vector.y * mat.m[1][0] + vector.z * mat.m[2][0] + 0.0f * mat.m[3][0];
	changeVector.y = vector.x * mat.m[0][1] + vector.y * mat.m[1][1] + vector.z * mat.m[2][1] + 0.0f * mat.m[3][1];
	changeVector.z = vector.x * mat.m[0][2] + vector.y * mat.m[1][2] + vector.z * mat.m[2][2] + 0.0f * mat.m[3][2];

	return changeVector;
}

Vector3 Player::VectorMat(Matrix4 mat, Vector3 vector)
{
	Vector3 changeVector = { 0,0,0 };

	changeVector.x = mat.m[0][0] * vector.x + mat.m[0][1] * vector.y + mat.m[0][2] * vector.z + mat.m[0][3] * 0.0f;
	changeVector.y = mat.m[1][0] * vector.x + mat.m[1][1] * vector.y + mat.m[1][2] * vector.z + mat.m[1][3] * 0.0f;
	changeVector.z = mat.m[2][0] * vector.x + mat.m[2][1] * vector.y + mat.m[2][2] * vector.z + mat.m[2][3] * 0.0f;

	return changeVector;
}