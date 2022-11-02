#pragma once
#include "WorldTransform.h"
#include "Model.h"
#include "Vector3.h"

class Effect
{
public:
	Effect();
	~Effect();

	/// <summary>
	/// ������
	/// </summary>
	/// <param name="pos">�����ꏊ</param>
	/// <param name="moveVec">���K�������ړ��x�N�g��</param>
	/// <param name="LiveTime">��ʂɑ��݂��鎞��</param>
	void Init(Model* model,Vector3 pos, Vector3 moveVec, float LiveTime = 50);

	/// <summary>
	/// �X�V
	/// </summary>
	
	void Update();
	void Draw(ViewProjection& viewProjection);

	void SetPos(Vector3 pos);
	void SetSpeed(float speed);

	bool GetIsDead() { return isDead; };

private:
	//���f��
	Model* model_ = nullptr;

	WorldTransform worldTransform;
	Vector3 moveVector;
	bool isDead = false;
	float liveTime = 50;
	float moveSpeed = 1;

};