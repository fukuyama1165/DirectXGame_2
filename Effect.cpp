#include "effect.h"
#include <cassert>

Effect::Effect()
{

}

Effect::~Effect()
{

}

void Effect::Init(Model* model,Vector3 pos, Vector3 moveVec, float LiveTime)
{
	//NULLポインタチェック
	assert(model);
	worldTransform.Initialize();

	model_ = model;
	
	SetPos(pos);
	liveTime = LiveTime;
	moveVector = moveVec;

}

void Effect::Update()
{
	liveTime--;
	if (liveTime <= 0)
	{
		isDead = true;
	}
	
	worldTransform.translation_ += moveVector * moveSpeed;
	worldTransform.matWorldGeneration();

}

void Effect::Draw(ViewProjection& viewProjection)
{

	model_->Draw(worldTransform, viewProjection);

}

void Effect::SetPos(Vector3 pos)
{

	worldTransform.translation_ = pos;

	worldTransform.matWorldGeneration();

}

void Effect::SetSpeed(float speed)
{

	moveSpeed = speed;

}