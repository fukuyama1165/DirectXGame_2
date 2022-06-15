#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include "DebugText.h"
#include "BaseEnemyState.h"
#include "EnemyBullet.h"
#include <memory>
#include <list>
#include "TimeCall.h"

class BaseEnemyState;

/// <summary>
/// �G
/// </summary>
class Enemy
{
public:
	Enemy();
	~Enemy();

	/// <summary>
	/// ������
	/// </summary>
	/// <param name="model">���f��</param>
	/// <param name="position">�������W</param>
	/// /// <param name="velocity">���x</param>
	void Initialize(Model* model, const Vector3& position, const Vector3& velocity);

	/// <summary>
	/// �X�V
	/// </summary>
	void Update();

	/// <summary>
	/// �`��
	/// </summary>
	/// <param name="viewProjection">�r���[�v���W�F�N�V����</param>
	void Draw(const ViewProjection& viewProjection);


	//�ڋ߂���Ƃ��̓��������֐�
	void ApproachMove();

	//���E����Ƃ��̓��������֐�
	void LeaveMove();

	//velocity��Translation�ړ�������֐�
	void MoveTranslation(Vector3 Velocity);

	//���݈ʒu��Ԃ��֐�
	Vector3 GetPos();
	
	//�X�^�C���ύX�p�֐�
	void ChangeState(BaseEnemyState* newStaete);

	//������Ԃ��֐�
	Enemy* getThis();

	//�e����
	void Fire();

	//fireTime���Z�b�g����֐�
	void SetFireTime(int32_t FireTime);

	//fireTime���擾����֐�
	int32_t GetFireTime();

	void FireAndReset();


private:

	//�����o�֐��|�C���^
	static void (Enemy::*PhaseMoveP[])();


public:

	static const int kFireInterval = 50;

private:

	//���[���h�g�����X�t�H�[��
	WorldTransform worldTransform_;

	//�e�N�X�`���n���h��
	uint32_t textureHandle_ = 0;

	//3D���f��
	Model* model_ = nullptr;

	//���x
	Vector3	Velocity_;

	DebugText* debugText_ = nullptr;

	enum class Phase {
		Approach,//�ڋ߂���
		Leave,//���E����
	};

	//�G�̋����ɂ���
	Phase phase_ = Phase::Approach;

	//���E����Ƃ��̃X�s�[�h
	Vector3 LeaveVelocity_;

	//�s���p�̃|�C���^
	BaseEnemyState *state_=nullptr;

	//�e
	std::list<std::unique_ptr<EnemyBullet>> bullets_;

	//���ˊԊu
	int32_t FireTime_ = 0;

	//std::list<std::unique_ptr<TimeCall>> timedCalls_;

};


