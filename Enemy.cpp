#include "Enemy.h"
#include <cassert>

Enemy::Enemy()
{
}

Enemy::~Enemy()
{
}

void Enemy::Initialize(Model* model, const Vector3& position, const Vector3& velocity)
{

	//ヌルポチェック
	assert(model);

	model_ = model;

	debugText_ = DebugText::GetInstance();

	//テクスチャハンドルを設定
	textureHandle_= TextureManager::Load("enemyBall.png");

	//ワールドトランスフォームの初期化
	worldTransform_.Initialize();

	//引数で受け取った初期座標をセット
	worldTransform_.translation_ = position;

	//引数で受け取った速度をセット
	Velocity_ = velocity;

	LeaveVelocity_ = { 0,0.1f,0.1f };

}

void Enemy::Update()
{
	//速度を足して移動する
	//worldTransform_.translation_ += Velocity_;

	switch (phase_)
	{
	case Enemy::Phase::Approach:

		//移動(ベクトルを加算)
		worldTransform_.translation_ += Velocity_;

		//規定の位置に到達したら離脱
		if (worldTransform_.translation_.z < 0.0f)
		{
			phase_ = Phase::Leave;
		}

		break;
	case Enemy::Phase::Leave:

		//移動(ベクトルを加算)
		worldTransform_.translation_ += LeaveVelocity_;

		break;
	default:
		break;
	}

	//行列の更新
	worldTransform_.matWorldGeneration();

}

void Enemy::Draw(const ViewProjection& viewProjection)
{
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
	debugText_->SetPos(50, 90);
	debugText_->Printf("Enemypos:(%f,%f,%f)", worldTransform_.translation_.x, worldTransform_.translation_.y, worldTransform_.translation_.z);
	debugText_->SetPos(50, 110);
	switch (phase_)
	{
	case Enemy::Phase::Approach:
		debugText_->Printf("Phase:Approach");
		break;
	case Enemy::Phase::Leave:
		debugText_->Printf("Phase:Leave");
		break;
	default:
		break;
	}

}