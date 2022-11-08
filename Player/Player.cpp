#include "Player.h"
#include <cassert>

void Player::Initialize(Model* model, Model* playerModel, uint32_t textureHandle)
{
	//NULLポインタチェック
	assert(model);
	assert(playerModel);

	model_ = model;
	playerModel_ = playerModel;
	textureHandle_ = textureHandle;


	//シングルトンインスタンスを取得する
	input_ = Input::GetInstance();
	debugText_ = DebugText::GetInstance();

	//ワールド変換の初期化
	worldTransform_.Initialize();

	worldTransform_.translation_ = { 0,0,0 };

	

	viewProjection_.Initialize();

}

Player::Player()
{
}

Player::~Player()
{
}

void Player::Update(ViewProjection viewProjection)
{
	Vector3 move = { 0,0,0 };

	Vector3 rota = { 0,0,0 };

	Vector3 kariZ = { 0,0,0 };

	float move_speed = 0.3f;



	bullets_.remove_if([](std::unique_ptr<PlayerBullet>& bullet)
		{
			return bullet->IsDead();
		});

	//移動
	if (input_->PushKey(DIK_UP))
	{
		move.z += move_speed;
	}
	if (input_->PushKey(DIK_DOWN))
	{
		move.z -= move_speed;
	}
	if (input_->PushKey(DIK_RIGHT))
	{
		move.x += move_speed;
	}
	if (input_->PushKey(DIK_LEFT))
	{
		move.x -= move_speed;

	}
	worldTransform_.translation_ += move;

	if (input_->PushKey(DIK_SPACE))
	{
		Attack();
	}

	for (std::unique_ptr<PlayerBullet>& bullet : bullets_)
	{
		bullet->Update();
	}
	worldTransform_.matWorldGeneration();
		
}

void Player::Draw(ViewProjection& viewProjection)
{

	//3Dモデルを描画
	playerModel_->Draw(worldTransform_, viewProjection);
	//model_->Draw(worldTransform3DReticle_, viewProjection);

	for(std::unique_ptr<PlayerBullet>& bullet:bullets_)
	{
		bullet->Draw(viewProjection);
	}

}





void Player::Attack()
{
	
	const float kBulletSpeed = 10.0f;

	Vector3 velocity(0, 0, kBulletSpeed);

	velocity = worldTransform_.matWorld_.VectorMat(velocity,worldTransform_.matWorld_);
	velocity.normalize();

	velocity *= kBulletSpeed;

	std::unique_ptr <PlayerBullet> newBullet = std::make_unique<PlayerBullet>();
	newBullet->Initlize(model_, worldTransform_.translation_, velocity);

	bullets_.push_back(std::move(newBullet));
	

}

Vector3 Player::VectorMat(Vector3 vector, Matrix4 mat)
{
	Vector3 changeVector = { 0,0,0 };

	changeVector.x = vector.x * mat.m[0][0] + vector.y * mat.m[1][0] + vector.z * mat.m[2][0] + 0.0f * mat.m[3][0];
	changeVector.y = vector.x * mat.m[0][1] + vector.y * mat.m[1][1] + vector.z * mat.m[2][1] + 0.0f * mat.m[3][1];
	changeVector.z = vector.x * mat.m[0][2] + vector.y * mat.m[1][2] + vector.z * mat.m[2][2] + 0.0f * mat.m[3][2];

	return changeVector;
}

Vector3 Player::VectorMat(Matrix4 mat, Vector3 vector)
{
	Vector3 changeVector = { 0,0,0 };

	changeVector.x = mat.m[0][0] * vector.x + mat.m[0][1] * vector.y + mat.m[0][2] * vector.z + mat.m[0][3] * 0.0f;
	changeVector.y = mat.m[1][0] * vector.x + mat.m[1][1] * vector.y + mat.m[1][2] * vector.z + mat.m[1][3] * 0.0f;
	changeVector.z = mat.m[2][0] * vector.x + mat.m[2][1] * vector.y + mat.m[2][2] * vector.z + mat.m[2][3] * 0.0f;

	return changeVector;
}

Vector3 Player::GetWorldPosition()
{
	//ワールド座標を入れる変数
	Vector3 worldpos;

	//ワールド行列の平行移動成分を取得(ワールド座標)
	worldpos.x = worldTransform_.matWorld_.m[3][0];
	worldpos.y = worldTransform_.matWorld_.m[3][1];
	worldpos.z = worldTransform_.matWorld_.m[3][2];

	return worldpos;

}

void Player::OnCollision()
{

}





void Player::reset()
{
	bullets_.remove_if([](std::unique_ptr<PlayerBullet>& bullet)//ifの中で簡易的な関数を生成してる->[](引数)
	{
		return 1;
	});

	worldTransform_.translation_ = { 0,0,50 };
	worldTransform_.matWorldGeneration();

}