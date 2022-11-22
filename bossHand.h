#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include "DebugText.h"

class bossHand
{
public:

	void init(Vector3 scale, Vector3 rotate, Vector3 translation,Model* model);

	void update(WorldTransform worldTransform);

	void draw(const ViewProjection& viewProjection);

	bool getisAttackFlag() { return isAttackFlag; };

	bool getisReturnHand() { return isReturnHand; };
	
	bool getisPressEndd() { return isPressEnd; };

	bool getisAction() { return isAction; };

	bool getisFallTargetMoveFlag() { return isFallTargetMoveFlag; };

	bool getisGetTargetPosFlag() { return isGetTargetPos; };

	void punch(WorldTransform worldTransform);

	void press();

	void stoneFall();
	void pillarFall();

	void beam();


	void pillarRoll();

	//����Ńt���O��true�ɂ�������Ƌ��������������Ȃ�̂Œ���(�g������߂�����)
	void setisActionFlag(bool flag);

	void setisAttackFlag(bool flag);

	void setisPressFlag(bool flag);

	void setisPressEndFlag(bool flag);

	void setisStoneFallFlag(bool flag);

	void setisPillarFallFlag(bool flag);

	void setisBeamFlag(bool flag);
	
	void setisPillarRollFlag(bool flag);

	//�v���C���[�̍U���ɂ���Ē��˕Ԃ��ꂽ���P�񂾂��s������
	void playerAttackReturn();

	void punchEnd();

	void setdefaultPos(Vector3 pos) { defaultPos = pos; };

	WorldTransform GetwroldTransform() { return Hand; }

	void setPos(Vector3 pos);

	void setScale(Vector3 scale);

	void setRotate(Vector3 rotate);

	void setTargetPos(Vector3 target);

private:
	WorldTransform Hand;

	//�肪�U������
	bool isAttackFlag = false;
	//�U�����I����Ė߂��Ă��鎞��
	bool isAttackReturnFlag = false;

	bool isFallTargetMoveFlag = false;
	bool isFallFallFlag = false;
	bool isFallReturnFlag = false;

	bool isPillarFallReturnFlag = false;

	bool isBeamFirstStart = false;

	bool isBeamEnd = false;

	bool isPillarRollFirstStart = false;
	

	bool isGetTargetPos = false;

	//��𒵂˕Ԃ������̃t���O
	bool isReturnHand = false;

	//�^�C���J�E���^�[
	float timeCount = 0;
	float returnTimeCount = 0;
	float returnAttackTimeCount = 0;
	float ActionType4TimeCount = 0;
	float ActionType5TimeCount = 0;

	//���쎞��
	float maxTime = 50.0f;
	float maxReturnTime = 20.0f;
	float maxReturnAttackTime = 10.0f;

	//�v���X
	float maxSetPressTime = 20.0f;
	float maxResetPressTime = 50.0f;

	//�Η��Ƃ�
	float maxTargetMoveTime = 35.0f;
	float maxFallTime = 10.0f;
	float maxFallReturnTime = 10.0f;
	float maxUpFallTime = 10.0f;

	//���i�K�p�Η��Ƃ�
	float maxUpPillarFallTime = 20.0f;

	//�r�[��
	float maxBeamTime = 30.0f;
	float maxFirstBeamTime = 20.0f;

	//��]
	float maxPillarRollTime = 30.0f;

	//�E�G�C�g
	float waitTime = 0;

	//�҂���
	float attackWaitTime = 40;
	float returnWaitTime = 160;

	float stoneFallWaitTime = 40;
	float stoneFallReturnWaitTime = 10;

	//���s�����Ă��邩
	bool isAction = false;
	bool isStoneFallAction = false;

	//
	bool isPress = false;
	bool isPressEnd = true;
	bool isStoneFall = false;
	bool isPillarFall = false;
	bool isPillarRoll = false;
	bool isBeam = false;

	//�v���C���[�̈ʒu
	Vector3 targetPos = {};

	//�U�����Ԃ��ꂽ�ʒu
	Vector3 returnPos = {};

	Vector3 defaultPos = {};

	Vector3 startBeamScale = { 5,5,5 };

	DebugText* debugText_ = nullptr;

	Model* model;

};

const Vector3 lerp(const Vector3& start, const Vector3& end, const float t);

