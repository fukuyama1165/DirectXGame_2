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
	matWorldGeneration(worldTransform_);

	debugText_->SetPos(50, 70);
	debugText_->Printf("pos:(%f,%f,%f)", worldTransform_.translation_.x, worldTransform_.translation_.y, worldTransform_.translation_.z);

}
void Player::Draw(ViewProjection& viewProjection)
{

	//3D���f����`��
	model_->Draw(worldTransform_, viewProjection, textureHandle_);

}

Matrix4 Player::matScaleGeneration(Vector3 scale)
{
	//�X�P�[�����O�s���錾
	Matrix4 matScale;

	//�X�P�[�����O�{�����s��ɐݒ�
	matScale.m[0][0] = scale.x;
	matScale.m[1][1] = scale.y;
	matScale.m[2][2] = scale.z;
	matScale.m[3][3] = 1;

	return matScale;
}

Matrix4 Player::matRotateXGeneration(float rotateX)
{
	//X����]�s���錾
	Matrix4 matRotateX;

	//��]�p���s��ɐݒ�(���W�A��)
	matRotateX.m[0][0] = 1;
	matRotateX.m[1][1] = cos(rotateX);
	matRotateX.m[1][2] = sin(rotateX);
	matRotateX.m[2][1] = -sin(rotateX);
	matRotateX.m[2][2] = cos(rotateX);
	matRotateX.m[3][3] = 1;

	return matRotateX;
}

Matrix4 Player::matRotateYGeneration(float rotateY)
{
	//Y����]�s���錾
	Matrix4 matRotateY;

	//��]�p���s��ɐݒ�(���W�A��)
	matRotateY.m[0][0] = cos(rotateY);
	matRotateY.m[0][2] = -sin(rotateY);
	matRotateY.m[1][1] = 1;
	matRotateY.m[2][0] = sin(rotateY);
	matRotateY.m[2][2] = cos(rotateY);
	matRotateY.m[3][3] = 1;

	return matRotateY;
}

Matrix4 Player::matRotateZGeneration(float rotateZ)
{
	//Z����]�s���錾
	Matrix4 matRotateZ;

	//��]�p���s��ɐݒ�(���W�A��)
	matRotateZ.m[0][0] = cos(rotateZ);
	matRotateZ.m[0][1] = sin(rotateZ);
	matRotateZ.m[1][0] = -sin(rotateZ);
	matRotateZ.m[1][1] = cos(rotateZ);
	matRotateZ.m[2][2] = 1;
	matRotateZ.m[3][3] = 1;

	return matRotateZ;
}

Matrix4 Player::matRotateGeneration(Vector3 rotate)
{
	//X����]�s���錾
	Matrix4 matRotateX = matRotateXGeneration(rotate.x);

	//Y����]�s���錾
	Matrix4 matRotateY = matRotateYGeneration(rotate.y);

	//Z����]�s���錾
	Matrix4 matRotateZ = matRotateZGeneration(rotate.z);

	//��]�������s���錾
	Matrix4 matrotate;

	//�v�Z�����p�x���v�Z(���Ԃ͉�]�����郂�f���ɂ���ĕς���)

	matRotateX *= matRotateY;

	matRotateZ *= matRotateX;

	matrotate = matRotateZ;

	return matrotate;
}

Matrix4 Player::matMoveGeneration(Vector3 move)
{
	//�ړ����邽�߂̍s���p��
	Matrix4 matMove = MathUtility::Matrix4Identity();

	//�s��Ɉړ��ʂ���
	matMove.m[3][0] = move.x;
	matMove.m[3][1] = move.y;
	matMove.m[3][2] = move.z;

	return matMove;
}

void Player::matWorldGeneration(WorldTransform& worldTransform)
{
	//�X�P�[�����O�s���錾
	Matrix4 matScale = matScaleGeneration(worldTransform.scale_);

	//��]�������s���錾
	Matrix4 matrotate = matRotateGeneration(worldTransform.rotation_);

	//�ړ����邽�߂̍s���p��
	Matrix4 matMove = matMoveGeneration(worldTransform.translation_);

	worldTransform.matWorld_ = MathUtility::Matrix4Identity();

	matrotate *= matMove;

	matScale *= matrotate;

	worldTransform.matWorld_ *= matScale;

	worldTransform.TransferMatrix();

}