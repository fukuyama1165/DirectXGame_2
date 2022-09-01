#pragma once
#include "WinApp.h"
#include "MathUtility.h"
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
	void Initialize(Model* model, Model* playerModel, uint32_t textureHandle);

	/// <summary>
	/// �X�V
	/// </summary>
	/// <param name="viewProjection">�r���[�v���W�F�N�V����(�Q�Ɠn��)</param>
	void Update(ViewProjection viewProjection);

	/// <summary>
	/// �`��
	/// </summary>
	/// <param name="viewProjection">�r���[�v���W�F�N�V����(�Q�Ɠn��)</param>
	void Draw(ViewProjection& viewProjection);

	/// <summary>
	/// UI�`��
	/// </summary>
	void DrawUI();

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

	//���[���h���W���擾
	Vector3 GetWorldPosition();

	//�Փ˂����o������Ăяo�����R�[���o�b�N�֐�
	void OnCollision();

	const std::list<std::unique_ptr<PlayerBullet>>& GetBullets() { return bullets_; };

	void SetCameraMat(Matrix4 CameraMat);

	//2D���e�B�N�����}�E�X�̈ʒu�ɒu���Ă����ɔ��˂���֐�
	void Reticle2DMouseAttack(ViewProjection viewProjection);

	void reset();

private:

	//���[���h�ϊ��f�[�^
	WorldTransform worldTransform_;

	ViewProjection viewProjection_;

	//���f��
	Model* model_ = nullptr;
	Model* playerModel_ = nullptr;

	//�e�N�X�`���n���h��
	uint32_t textureHandle_ = 0u;

	Input* input_ = nullptr;

	DebugText* debugText_ = nullptr;

	//�e
	std::list<std::unique_ptr<PlayerBullet>> bullets_;

	Matrix4 CameraMat_ = {};

	//3D���e�B�N���p���[���h�g�����X�t�H�[��
	WorldTransform worldTransform3DReticle_;

	//2d���e�B�N���p�X�v���C�g
	std::unique_ptr<Sprite> sprite2DReticle_;

	int bulletCT = 0;

	const int MAXBulletCT = 5;

};

