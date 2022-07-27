#include "Skydome.h"

void Skydome::Initialize(Model* model)
{
	worldTransform_.Initialize();

	worldTransform_.scale_ = { 1000.0f,1000.0f,1000.0f };

	worldTransform_.matWorldGeneration();

	model_ = model;
}

void Skydome::Update()
{

}

void Skydome::Draw(ViewProjection& viewProjection)
{

	//3Dƒ‚ƒfƒ‹•`‰æ
	model_->Draw(worldTransform_, viewProjection);

}