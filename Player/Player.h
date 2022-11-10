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
#include"RailCamera.h"

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
	void Initialize(Model* model, Model* playerModel, uint32_t textureHandle,RailCamera* camera);

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
	
	/// <summary>
	/// �U��
	/// </summary>
	void Attack(Vector3 flont);

	/// <summary>
	/// �s��ƃx�N�g���̌v�Z(�������s��v�Z�̐�)
	/// </summary>
	Vector3 VectorMat(Vector3 vector, Matrix4 mat);

	/// <summary>
	/// �s��ƃx�N�g���̌v�Z(�������s��v�Z�̐�)
	/// </summary>
	Vector3 VectorMat(Matrix4 mat, Vector3 vector);

	Vector3 PlayerMoveVec();

	//���[���h���W���擾
	Vector3 GetWorldPosition();

	WorldTransform GetMat();

	void SetWorldPosition(Vector3 osimodosi);

	//�Փ˂����o������Ăяo�����R�[���o�b�N�֐�
	void OnCollision();

	const std::list<std::unique_ptr<PlayerBullet>>& GetBullets() { return bullets_; };


	void reset();

private:

	RailCamera* camera = nullptr;

	//���[���h�ϊ��f�[�^
	WorldTransform worldTransform_;
	WorldTransform target;

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


	int bulletCT = 0;

	const int MAXBulletCT = 5;

	Vector3 moveVec;

	float move_speed;

	bool hopper_dash;

	int cooltime;

	int yuyotime;

	float hopper_speed;

	bool cooldown;

	Vector3 dash_vec;

	bool B_bottan;
	bool old_B_bottan;

};

