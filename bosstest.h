#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include "DebugText.h"
#include "bossHand.h"
#include <memory>
#include <vector>

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
	void Update(Vector3 player);

	/// <summary>
	/// �`��
	/// </summary>
	/// <param name="viewProjection">�r���[�v���W�F�N�V����</param>
	void Draw(const ViewProjection& viewProjection);

	//���[���h���W���擾
	Vector3 GetWorldPosition();

	void setPos(Vector3 pos);

	void setisAttackFlagL(bool flag);

	void setisBossPress(bool flag);

	void playerAttackReturnL();

	void bossPress(Vector3 player);

	WorldTransform getPos() { return worldTransform; };

	std::vector<bossHand*> getHand() { return hand; };

	void setPressPos();

	void endPress();

	void setPressHandPos();

private:

	DebugText* debugText_ = nullptr;

	//���[���h�g�����X�t�H�[��
	WorldTransform worldTransform;

	//��̍��W(�����������珈����������������Ȃ��̂ŃN���X�������ق�����������)
	/*bossHand hand[8];*/

	//�N���X������
	std::vector<bossHand*> hand;

	enum phase
	{
		Cube,
		pillar,
		ophanim
	};
	
	int state = Cube;

	//�e�N�X�`���n���h��
	uint32_t textureHandle_ = 0;

	//3D���f��
	Model* model_ = nullptr;

	bool isHandMove = false;

	bool isBossPress = false;

	bool isPressStart = false;
	bool isPressFall = false;
	bool isPressReturn = false;
	bool pressFirstStart = false;

	int pressCount = 0;

	float pressPosY = 20;

	Vector3 returnPos = {};
	Vector3 targetPos = {};

	//�^�C���J�E���^�[
	float timeCount = 0;
	float fallTimeCount = 0;
	float returnTimeCount = 0;

	//���쎞��
	float maxTime = 300.0f;
	float maxfallTime = 500.0f;
	float maxReturnTime = 200.0f;

	//�E�G�C�g
	float waitTime = 0;

	//�҂���
	float fallWaitTime = 20;
	float returnWaitTime = 700;

};

