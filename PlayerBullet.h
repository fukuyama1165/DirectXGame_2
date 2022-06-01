#pragma once
#include "Model.h"
#include "WorldTransform.h"

/// <summary>
/// ���L�����̒e
/// </summary>
class PlayerBullet
{
public:
	PlayerBullet();
	~PlayerBullet();

	/// <summary>
	/// ������
	/// </summary>
	/// <param name="model">���f��</param>
	/// <param name="position">�������W</param>
	void Initlize(Model* model, const Vector3& position);

	/// <summary>
	/// �X�V
	/// </summary>
	void Update();

	/// <summary>
	/// �`��
	/// </summary>
	/// <param name="viewProjection">�r���[�v���W�F�N�V����</param>
	void Draw(const ViewProjection& viewProjection);

private:

	//���[���h�ϊ��f�[�^
	WorldTransform worldTransform_;

	//�e�N�X�`���n���h��
	uint32_t textureHandle_ = 0;

	//3D���f��(�؂�Ă����炵��)
	Model* model_ = nullptr;

};

