#include "WorldTransform.h"
#include "LightGroup.h"
#include "DebugText.h"


Matrix4 WorldTransform::matScaleGeneration(Vector3 scale)
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

Matrix4 WorldTransform::matRotateXGeneration(float rotateX)
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

Matrix4 WorldTransform::matRotateYGeneration(float rotateY)
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

Matrix4 WorldTransform::matRotateZGeneration(float rotateZ)
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

Matrix4 WorldTransform::matRotateGeneration(Vector3 rotate)
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

Matrix4 WorldTransform::matMoveGeneration(Vector3 move)
{
	//�ړ����邽�߂̍s���p��
	Matrix4 matMove = MathUtility::Matrix4Identity();

	//�s��Ɉړ��ʂ���
	matMove.m[3][0] = move.x;
	matMove.m[3][1] = move.y;
	matMove.m[3][2] = move.z;

	return matMove;
}

void WorldTransform::matWorldGeneration(WorldTransform& worldTransform)
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