#pragma once

#include "Model.h"
#include "WorldTransform.h"
#include "MathUtility.h"

/// <summary>
/// ���L�����̒e
/// </summary>
class EnemyBullet
{
public:
	EnemyBullet();
	~EnemyBullet();

	/// <summary>
	/// ������
	/// </summary>
	/// <param name="model">���f��</param>
	/// <param name="position">�������W</param>
	/// <param name="velocity">���x</param>
	void Initlize(Model* model, const Vector3& position, const Vector3& velocity);

	/// <summary>
	/// �X�V
	/// </summary>
	void Update();

	/// <summary>
	/// �`��
	/// </summary>
	/// <param name="viewProjection">�r���[�v���W�F�N�V����</param>
	void Draw(const ViewProjection& viewProjection);

	bool IsDead()const { return isDead_; }

private:

	//���[���h�ϊ��f�[�^
	WorldTransform worldTransform_;

	//�e�N�X�`���n���h��
	uint32_t textureHandle_ = 0;

	//3D���f��(�؂�Ă����炵��)
	Model* model_ = nullptr;

	//���x
	Vector3	Velocity_;

	//����<frm>
	static const int32_t kLifeTime = 60 * 5;

	//�f�X�^�C�}�[
	int32_t deathTimer_ = kLifeTime;

	//�f�X�t���O
	bool isDead_ = false;

};

