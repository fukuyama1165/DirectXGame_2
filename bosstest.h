#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include "DebugText.h"

class bosstest
{
public:
	bosstest();
	~bosstest();

	/// <summary>
	/// ������
	/// </summary>
	/// <param name="model">���f��</param>
	/// <param name="position">�������W</param>
	void Initialize(Model* model, const Vector3& position);

	/// <summary>
	/// �X�V
	/// </summary>
	void Update();

	/// <summary>
	/// �`��
	/// </summary>
	/// <param name="viewProjection">�r���[�v���W�F�N�V����</param>
	void Draw(const ViewProjection& viewProjection);

	//���[���h���W���擾
	Vector3 GetWorldPosition();

	void setPos(Vector3 pos);

	bool getisAttackFlagL() { return isAttackFlagL; };
	bool getisAttackFlagR() { return isAttackFlagR; };

	bool getisReturnHandL() { return isReturnHandL; };
	bool getisReturnHandR() { return isReturnHandR; };

	void setisAttackFlagL(bool flag);
	void setisAttackFlagR(bool flag);

	//�v���C���[�̍U���ɂ���Ē��˕Ԃ��ꂽ���P�񂾂��s������
	void playerAttackReturnL();
	void playerAttackReturnR();

	void attackEnd();

private:

	DebugText* debugText_ = nullptr;

	//���[���h�g�����X�t�H�[��
	WorldTransform worldTransform;

	//��̍��W(�����������珈����������������Ȃ��̂ŃN���X�������ق�����������)
	WorldTransform leftHand;
	WorldTransform rightHand;

	//�肪�U������
	bool isAttackFlagL = false;
	bool isAttackFlagR = false;
	//�U�����I����Ė߂��Ă��鎞��
	bool isAttackReturnFlagL = false;
	bool isAttackReturnFlagR = false;

	//��𒵂˕Ԃ������̃t���O
	bool isReturnHandL = false;
	bool isReturnHandR = false;

	
	//�e�N�X�`���n���h��
	uint32_t textureHandle_ = 0;

	//3D���f��
	Model* model_ = nullptr;

	float timeCount = 0;
	float returnTimeCount = 0;
	float returnAttackTimeCount = 0;

	float maxTime = 500.0f;
	float maxReturnTime = 500.0f;
	float maxReturnAttackTime = 200.0f;

	Vector3 returnPos = {};
};

