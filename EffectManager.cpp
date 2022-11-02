#include "EffectManager.h"
#include <stdlib.h>
#include <time.h>
#include <random>

EffectManager::EffectManager()
{
}

EffectManager::~EffectManager()
{


	delete model;

}

void EffectManager::Init()
{
	model = Model::Create();
	worldTransform.Initialize();
}

void EffectManager::Update()
{

	for (int i = 0; i < effects.size(); i++)
	{
		if (effects[i]->GetIsDead())
		{
			effects.erase(effects.begin() + i);
		}
	}

	for (int i = 0; i < effects.size(); i++)
	{
		effects[i]->Update();
	}

}

void EffectManager::ExplosionEffect(int activeTime)
{
	if (isStart == false)
	{
		activeTimer = activeTime;
		isStart = true;
	}


	if (activeTimer <= 0 and isEffectEnd == false)
	{
		isEffectEnd = true;
	}

	if (isEffectEnd == false)
	{

		if (coolTime <= 0)
		{
			float x = GetRand(-10,10);
			float y = GetRand(-10,10);
			moveVecor = { x,y,0 };
			moveVecor.normalize();
			popEffect(worldTransform.matWorldGetPos(), 10);
			coolTime = maxCoolTime;
		}

		coolTime--;
		activeTimer--;

	}

}

void EffectManager::popEffect(Vector3 pos,float liveTime)
{

	std::unique_ptr<Effect> newEffect = std::make_unique<Effect>();
	newEffect->Init(model, pos, moveVecor,liveTime);
	newEffect->SetSpeed(effctSpeed);
	
	//“GƒLƒƒƒ‰‚ð“o˜^
	effects.push_back(std::move(newEffect));

}

void EffectManager::setPos(Vector3 pos)
{

	worldTransform.translation_ = pos;

	worldTransform.matWorldGeneration();

}

WorldTransform EffectManager::getWorldTransform()
{

	return worldTransform;

}

void EffectManager::draw(ViewProjection& viewProjection)
{

	for (int i = 0; i < effects.size(); i++)
	{
		effects[i]->Draw(viewProjection);
	}

}

int GetRand(int min, int max)
{

	std::random_device rd;
	std::default_random_engine eng(rd());
	std::uniform_int_distribution<> distr(min, max);
	return distr(eng);

}

float GetRand(float maxRand)
{

	srand(time(nullptr));

	int maxrand = maxRand;

	float ans = rand() % maxrand;

	return ans;

}