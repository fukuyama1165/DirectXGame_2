#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include "DebugText.h"
#include "Input.h"
#include "PlayerBullet.h"
#include <memory>
#include <list>

/// <summary>
/// ���L����
/// </summary>
class Player
{
public:
	Player();
	~Player();
	/// <summary>
	/// ������
	/// </summary>
	/// <param name="model">���f��</param>
	/// <param name="textureHandle">�e�N�X�`���n���h��</param>
	void Initialize(Model* model, uint32_t textureHandle);

	/// <summary>
	/// �X�V
	/// </summary>
	void Update();

	/// <summary>
	/// �`��
	/// </summary>
	/// <param name="viewProjection">�r���[�v���W�F�N�V����(�Q�Ɠn��)</param>
	void Draw(ViewProjection& viewProjection);

	//��](arrow�L�[�̍��E�ŕύX)
	void Rotate();

	/// <summary>
	/// �U��
	/// </summary>
	void Attack();

	/// <summary>
	/// �s��ƃx�N�g���̌v�Z(�������s��v�Z�̐�)
	/// </summary>
	Vector3 VectorMat(Vector3 vector, Matrix4 mat);

	/// <summary>
	/// �s��ƃx�N�g���̌v�Z(�������s��v�Z�̐�)
	/// </summary>
	Vector3 VectorMat(Matrix4 mat, Vector3 vector);

private:

	//���[���h�ϊ��f�[�^
	WorldTransform worldTransform_;

	//���f��
	Model* model_ = nullptr;

	//�e�N�X�`���n���h��
	uint32_t textureHandle_ = 0u;

	Input* input_ = nullptr;

	DebugText* debugText_ = nullptr;

	//�e
	std::list<std::unique_ptr<PlayerBullet>> bullets_;

};
