#include "RailCamera.h"

RailCamera::RailCamera()
{

}
RailCamera::~RailCamera()
{

}

void RailCamera::Initialize(Vector3 worldPos, Vector3 Rot)
{
	//���[���h�g�����X�t�H�[���̏����ݒ�
	worldTransform_.Initialize();

	worldTransform_.translation_ = worldPos;
	worldTransform_.rotation_ = Rot;
	worldTransform_.scale_ = {1.0f,1.0f,1.0f};

	

	//�r���[�v���W�F�N�V�����̏�����
	viewProjection_.farZ = 1000000.0f;
	viewProjection_.Initialize();

	debugText_ = DebugText::GetInstance();

}

void RailCamera::Update()
{

	worldTransform_.matWorldGeneration();

	viewProjection_.eye = worldTransform_.matWorldGetPos();

	//���[���h�O���x�N�g��
	forward = { 0.0f, 0.0f, 1.0f };

	//���[���J�����̉�]�𔽉f
	forward = VectorMat(forward, worldTransform_.matWorld_);

	//���_����O���ɓK���ȋ����i�񂾈ʒu�������_
	viewProjection_.target = viewProjection_.eye + forward;

	//���[���h����x�N�g��
	Vector3 up(0, 1, 0);

	//���[���J�����̉�]�𔽉f(���[���J�����̏���x�N�g��)
	viewProjection_.up = VectorMat(up, worldTransform_.matWorld_);

	viewProjection_.UpdateMatrix();
	viewProjection_.TransferMatrix();

	/*debugText_->SetPos(50, 110);
	debugText_->Printf("eye:(%f,%f,%f)", viewProjection_.eye.x, viewProjection_.eye.y, viewProjection_.eye.z);*/
}

Vector3 RailCamera::VectorMat(Vector3 vector, Matrix4 mat)
{
	Vector3 changeVector = { 0,0,0 };

	changeVector.x = vector.x * mat.m[0][0] + vector.y * mat.m[1][0] + vector.z * mat.m[2][0] + 0.0f * mat.m[3][0];
	changeVector.y = vector.x * mat.m[0][1] + vector.y * mat.m[1][1] + vector.z * mat.m[2][1] + 0.0f * mat.m[3][1];
	changeVector.z = vector.x * mat.m[0][2] + vector.y * mat.m[1][2] + vector.z * mat.m[2][2] + 0.0f * mat.m[3][2];

	return changeVector;
}

Vector3 RailCamera::VectorMat(Matrix4 mat, Vector3 vector)
{
	Vector3 changeVector = { 0,0,0 };

	changeVector.x = mat.m[0][0] * vector.x + mat.m[0][1] * vector.y + mat.m[0][2] * vector.z + mat.m[0][3] * 0.0f;
	changeVector.y = mat.m[1][0] * vector.x + mat.m[1][1] * vector.y + mat.m[1][2] * vector.z + mat.m[1][3] * 0.0f;
	changeVector.z = mat.m[2][0] * vector.x + mat.m[2][1] * vector.y + mat.m[2][2] * vector.z + mat.m[2][3] * 0.0f;

	return changeVector;
}

ViewProjection RailCamera::getView()
{
	return viewProjection_;
}

Matrix4 RailCamera::getMatWorld()
{
	return worldTransform_.matWorld_;
}

void RailCamera::setPos(Vector3 pos)
{
	worldTransform_.translation_ = pos;
	worldTransform_.matWorldGeneration();
}

void RailCamera::setRotate(Vector3 rotate)
{
	worldTransform_.rotation_ = rotate;
	worldTransform_.matWorldGeneration();
}

void RailCamera::setWorldMat(Matrix4 woeldMat)
{
	worldTransform_.matWorld_ = woeldMat;
}

Vector3 RailCamera::GetWorldPosition()
{

	//���[���h���W������ϐ�
	Vector3 worldpos;

	//���[���h�s��̕��s�ړ��������擾(���[���h���W)
	worldpos.x = worldTransform_.matWorld_.m[3][0];
	worldpos.y = worldTransform_.matWorld_.m[3][1];
	worldpos.z = worldTransform_.matWorld_.m[3][2];

	return worldpos;

}

Vector3 RailCamera::getForwardVec()
{
	return forward.normalize();
}