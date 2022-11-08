#pragma once
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "DebugText.h"


class RailCamera
{

public:

	RailCamera();
	~RailCamera();
	
	/// <summary>
	/// ������
	/// </summary>
	/// <param name="worldTransform">���[���h���W</param>
	/// <param name="Rot">��]�p(���W�A��)</param>
	void Initialize(Vector3 worldPos,Vector3 Rot);

	/// <summary>
	/// �X�V
	/// </summary>
	void Update();

	/// <summary>
	/// �s��ƃx�N�g���̌v�Z(�������s��v�Z�̐�)
	/// </summary>
	Vector3 VectorMat(Vector3 vector, Matrix4 mat);

	/// <summary>
	/// �s��ƃx�N�g���̌v�Z(�������s��v�Z�̐�)
	/// </summary>
	Vector3 VectorMat(Matrix4 mat, Vector3 vector);

	ViewProjection getView();

	Matrix4 getMatWorld();

	Vector3 getForwardVec();

	//���[���h���W���擾
	Vector3 GetWorldPosition();

	void setPos(Vector3 pos);

	void setRotate(Vector3 rotate);

	void setWorldMat(Matrix4 woeldMat);

private:

	//���[���h�ϊ��f�[�^
	WorldTransform worldTransform_;

	//�r���[�v���W�F�N�V����
	ViewProjection viewProjection_;

	DebugText* debugText_ = nullptr;

	Vector3 forward = { 0.0f, 0.0f, 1.0f };

};

