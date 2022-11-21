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

	//�{�X�{�̂̍��W��ς���
	void setPos(Vector3 pos);

	void setRotate(Vector3 rotate);

	//hand�̃p���`�����邽�߂̃t���O��ύX
	void setisAttackFlagL(bool flag,Vector3 player);

	//�v���X�U�������邽�߂̃t���O��ύX
	void setisBossPress(bool flag);

	//�Η��Ƃ������邽�߂̃t���O��ύX
	void setisBossStoneFall(bool flag);

	void setisBossBeam(bool flag);

	//���̏�Ńp���`�̍U���𒵂˕Ԃ������Ƃɂ���
	void playerAttackReturnL();

	//�v���X�U��
	void bossPress(Vector3 player);

	//�Η��Ƃ�
	void bossStoneFall(Vector3 player);

	void bossBeam();

	//pos???????????? �S�������Ă���
	WorldTransform getPos() { return worldTransform; };

	//hand�������Ă���
	std::vector<bossHand*> getHand() { return hand; };

	//�v���X�U��������Ƃ��̖ڕW���W�ړ��p�֐�
	void setPressPos();

	//�{�̂ƈꏏ��hand���������߂̊֐�
	void setPressHandPos();
	void setPillarHandPos();

	void setCubeDefaultPos();
	void setPillarDefaultPos();


	void setPressEnd();



	int getHP() { return bossHP; };

	//�^����_���[�W������(�_���[�W���m�肵�����p�Ŋ֐�����邱��)
	void OnCollision(int damage);

private:

	DebugText* debugText_ = nullptr;

	//���[���h�g�����X�t�H�[��
	WorldTransform worldTransform;

	int bossHP = 100;

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
	
	int state = pillar;

	float setbossCubeDistance = 6.0f;
	float setbossCubePressDistance = 4.8f;

	//�e�N�X�`���n���h��
	uint32_t textureHandle_ = 0;

	//3D���f��
	Model* model_ = nullptr;

	bool isHandMove = false;

	bool isBossPress = false;
	bool isBossStoneFall = false;
	bool isBossBeam = false;

	bool isPressStart = false;
	bool isPressFall = false;
	bool isPressReturn = false;
	bool pressFirstStart = false;
	bool beamFirstStart = false;
	bool beamEndStart = false;

	int pressCount = 0;
	int bossStoneFallCount = 0;
	int bossBeamCount = 0;

	float pressPosY = 20;

	Vector3 returnPos = {};
	Vector3 targetPos = {};
	Vector3 beamStartPos = {};

	//�^�C���J�E���^�[
	float timeCount = 0;
	float fallTimeCount = 0;
	float returnTimeCount = 0;
	float moveEndTimeCount = 0;

	float beamFirstMoveCount = 0;
	float beamEndMoveCount = 0;

	//���쎞��
	float maxTime = 10.0f;
	float maxfallTime = 10.0f;
	float maxReturnTime = 20.0f;
	float maxMoveEndTime = 5.0f;

	float maxFirstBeamMoveTime = 20.0f;
	float maxEndBeamMoveTime = 20.0f;

	//�E�G�C�g
	float waitTime = 0;

	//�҂���
	float fallWaitTime = 2;
	float returnWaitTime = 20;

	float bossStoneFallWaitTime = 20;

	float bossBeamWaitTime = 40;

	float bosspillarMoveTime = 0;
	float maxBosspillarMoveTime = 10;
	int bosspillarDefaultPosCount = 0;

	Vector3 pillarDefaultPosRotate[8] =
	{
		{ 0,0,1 },
		{ 1,0,1 },
		{ 1,0,0 },
		{ 1,0,-1 },
		{ 0,0,-1 },
		{ -1,0,-1 },
		{ -1,0,0 },
		{ -1,0,1 },
	};

};

