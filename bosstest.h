#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include "DebugText.h"
#include "bossHand.h"

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

	void setisAttackFlagL(bool flag);

	void playerAttackReturnL();

private:

	DebugText* debugText_ = nullptr;

	//���[���h�g�����X�t�H�[��
	WorldTransform worldTransform;

	//��̍��W(�����������珈����������������Ȃ��̂ŃN���X�������ق�����������)
	bossHand Hand[8];

	
	//�e�N�X�`���n���h��
	uint32_t textureHandle_ = 0;

	//3D���f��
	Model* model_ = nullptr;


};

