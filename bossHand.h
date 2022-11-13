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

	void punch(WorldTransform worldTransform);

	void press(WorldTransform worldTransform);

	void setisAttackFlag(bool flag);

	void setisPressFlag(bool flag);

	//�v���C���[�̍U���ɂ���Ē��˕Ԃ��ꂽ���P�񂾂��s������
	void playerAttackReturn();

	void punchEnd();

	void setdefaultPos(Vector3 pos) { defaultPos = pos; };

	WorldTransform GetwroldTransform() { return Hand; }

	void setPos(Vector3 pos);

	void setScale(Vector3 scale);

	void setTargetPos(Vector3 target);

private:
	WorldTransform Hand;

	//�肪�U������
	bool isAttackFlag = false;
	//�U�����I����Ė߂��Ă��鎞��
	bool isAttackReturnFlag = false;

	//��𒵂˕Ԃ������̃t���O
	bool isReturnHand = false;

	//�^�C���J�E���^�[
	float timeCount = 0;
	float returnTimeCount = 0;
	float returnAttackTimeCount = 0;

	//���쎞��
	float maxTime = 500.0f;
	float maxReturnTime = 500.0f;
	float maxReturnAttackTime = 200.0f;

	float maxSetPressTime = 20.0f;

	//�E�G�C�g
	float waitTime = 0;

	//�҂���
	float attackWaitTime = 40;
	float returnWaitTime = 160;

	//���s�����Ă��邩
	bool isAction = false;

	bool isPress = false;

	//�v���C���[�̈ʒu
	Vector3 targetPos = {};

	//�U�����Ԃ��ꂽ�ʒu
	Vector3 returnPos = {};

	Vector3 defaultPos = {};

	DebugText* debugText_ = nullptr;

	Model* model;

};

const Vector3 lerp(const Vector3& start, const Vector3& end, const float t);

