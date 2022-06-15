#include "Player.h"
#include <cassert>

void Player::Initialize(Model* model, uint32_t textureHandle)
{
	//NULLポインタチェック
	assert(model);

	model_ = model;
	textureHandle_ = textureHandle;

	//シングルトンインスタンスを取得する
	input_ = Input::GetInstance();
	debugText_ = DebugText::GetInstance();

	//ワールド変換の初期化
	worldTransform_.Initialize();
}

Player::Player()
{
}

Player::~Player()
{
}

void Player::Update()
{

	//デスフラグの立った弾を削除(remove_if->条件一致を全て削除)
	bullets_.remove_if([](std::unique_ptr<PlayerBullet>& bullet)//ifの中で簡易的な関数を生成してる->[](引数)
	{
		return bullet->IsDead();
	});

	//キャラクターの移動ベクトル
	Vector3 move = { 0,0,0 };

	const float MoveSpeed = 1;

	if (input_->PushKey(DIK_W))
	{
		move = { 0,MoveSpeed,0 };
	}

	if (input_->PushKey(DIK_S))
	{
		move = { 0,-MoveSpeed,0 };
	}

	if (input_->PushKey(DIK_D))
	{
		move = { MoveSpeed,0,0 };
	}

	if (input_->PushKey(DIK_A))
	{
		move = { -MoveSpeed,0,0 };
	}

	//移動限界座標
	const float kMoveLimitX = 30;
	const float kMoveLimitY = 15;

	worldTransform_.translation_ += move;

	worldTransform_.translation_.x = max(worldTransform_.translation_.x, -kMoveLimitX);
	worldTransform_.translation_.x = min(worldTransform_.translation_.x, +kMoveLimitX);

	worldTransform_.translation_.y = max(worldTransform_.translation_.y, -kMoveLimitY);
	worldTransform_.translation_.y = min(worldTransform_.translation_.y, +kMoveLimitY);

	//座標移動(行列更新もこの関数で)
	worldTransform_.matWorldGeneration();

	//回転処理arrowキーの左右で回転させる
	Rotate();

	//Spaceキーで弾を生成してplayerの正面に進ませる
	Attack();

	for (std::unique_ptr<PlayerBullet>& bullet : bullets_)
	{
		bullet->Update();
	}

	debugText_->SetPos(50, 70);
	debugText_->Printf("pos:(%f,%f,%f)", worldTransform_.translation_.x, worldTransform_.translation_.y, worldTransform_.translation_.z);

}

void Player::Draw(ViewProjection& viewProjection)
{

	//3Dモデルを描画
	model_->Draw(worldTransform_, viewProjection, textureHandle_);

	for(std::unique_ptr<PlayerBullet>& bullet:bullets_)
	{
		bullet->Draw(viewProjection);
	}

}

void Player::Rotate()
{
	//キャラクターの移動ベクトル
	Vector3 rotate = { 0,0,0 };

	const float MoveSpeed = 0.1;

	if (input_->PushKey(DIK_RIGHT))
	{
		rotate = { 0,MoveSpeed,0 };
	}

	if (input_->PushKey(DIK_LEFT))
	{
		rotate = { 0,-MoveSpeed,0 };
	}

	worldTransform_.rotation_ += rotate;

	//座標移動(行列更新もこの関数で)
	worldTransform_.matWorldGeneration();
}

void Player::Attack()
{
	if (input_->TriggerKey(DIK_SPACE))
	{
		//発射地点の為に自キャラの座標をコピー
		Vector3 position = worldTransform_.translation_;

		//移動量を追加
		const float kBulletSpeed = 1.0f;
		Vector3 velocity(0, 0, kBulletSpeed);

		//速度ベクトルを自機の向きに合わせて回転する
		velocity = VectorMat(velocity, worldTransform_.matWorld_);

		//弾の生成と初期化
		std::unique_ptr<PlayerBullet> newBullet = std::make_unique<PlayerBullet>();
		newBullet->Initlize(model_, position,velocity);

		//弾を登録
		bullets_.push_back(std::move(newBullet));
	}
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