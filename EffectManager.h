#pragma once
#include "Effect.h"
#include <vector>
#include "Vector3.h"
#include "Model.h"
#include "WorldTransform.h"

class EffectManager
{
public:
	EffectManager();
	~EffectManager();

	void Init();
	void Update();

	void ExplosionEffect(int activeTime);
	void popEffect(Vector3 pos, float liveTime = 50);

	void setPos(Vector3 pos);

	void draw(ViewProjection& viewProjection);

	WorldTransform getWorldTransform();

	bool GetIsEffctEnd() { return isEffectEnd; };


private:

	std::vector<std::unique_ptr<Effect>> effects;

	//���s���Ԃ��I�������true�ɂȂ�Flag
	bool isEffectEnd = false;

	WorldTransform worldTransform;

	Model* model = nullptr;

	//�P�̂��ǂ����ɂɓ�����������ϐ�
	Vector3 moveVecor = {};

	//�n�܂�����(�^�C�}�[�Z�b�g�̂���)
	bool isStart = false;
	//�G�t�F�N�g�𓮂�������
	int activeTimer = 0;

	
	const int maxCoolTime = 3;

	int coolTime = maxCoolTime;

	float effctSpeed = 0.5f;

};

int GetRand(int min, int max);
float GetRand(float maxRand);
