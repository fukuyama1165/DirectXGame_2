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
	matWorldGeneration(worldTransform_);

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

Matrix4 Player::matScaleGeneration(Vector3 scale)
{
	//スケーリング行列を宣言
	Matrix4 matScale;

	//スケーリング倍率を行列に設定
	matScale.m[0][0] = scale.x;
	matScale.m[1][1] = scale.y;
	matScale.m[2][2] = scale.z;
	matScale.m[3][3] = 1;

	return matScale;
}

Matrix4 Player::matRotateXGeneration(float rotateX)
{
	//X軸回転行列を宣言
	Matrix4 matRotateX;

	//回転角を行列に設定(ラジアン)
	matRotateX.m[0][0] = 1;
	matRotateX.m[1][1] = cos(rotateX);
	matRotateX.m[1][2] = sin(rotateX);
	matRotateX.m[2][1] = -sin(rotateX);
	matRotateX.m[2][2] = cos(rotateX);
	matRotateX.m[3][3] = 1;

	return matRotateX;
}

Matrix4 Player::matRotateYGeneration(float rotateY)
{
	//Y軸回転行列を宣言
	Matrix4 matRotateY;

	//回転角を行列に設定(ラジアン)
	matRotateY.m[0][0] = cos(rotateY);
	matRotateY.m[0][2] = -sin(rotateY);
	matRotateY.m[1][1] = 1;
	matRotateY.m[2][0] = sin(rotateY);
	matRotateY.m[2][2] = cos(rotateY);
	matRotateY.m[3][3] = 1;

	return matRotateY;
}

Matrix4 Player::matRotateZGeneration(float rotateZ)
{
	//Z軸回転行列を宣言
	Matrix4 matRotateZ;

	//回転角を行列に設定(ラジアン)
	matRotateZ.m[0][0] = cos(rotateZ);
	matRotateZ.m[0][1] = sin(rotateZ);
	matRotateZ.m[1][0] = -sin(rotateZ);
	matRotateZ.m[1][1] = cos(rotateZ);
	matRotateZ.m[2][2] = 1;
	matRotateZ.m[3][3] = 1;

	return matRotateZ;
}

Matrix4 Player::matRotateGeneration(Vector3 rotate)
{
	//X軸回転行列を宣言
	Matrix4 matRotateX = matRotateXGeneration(rotate.x);

	//Y軸回転行列を宣言
	Matrix4 matRotateY = matRotateYGeneration(rotate.y);

	//Z軸回転行列を宣言
	Matrix4 matRotateZ = matRotateZGeneration(rotate.z);

	//回転軸合成行列を宣言
	Matrix4 matrotate;

	//計算した角度を計算(順番は回転させるモデルによって変える)

	matRotateX *= matRotateY;

	matRotateZ *= matRotateX;

	matrotate = matRotateZ;

	return matrotate;
}

Matrix4 Player::matMoveGeneration(Vector3 move)
{
	//移動するための行列を用意
	Matrix4 matMove = MathUtility::Matrix4Identity();

	//行列に移動量を代入
	matMove.m[3][0] = move.x;
	matMove.m[3][1] = move.y;
	matMove.m[3][2] = move.z;

	return matMove;
}

void Player::matWorldGeneration(WorldTransform& worldTransform)
{
	//スケーリング行列を宣言
	Matrix4 matScale = matScaleGeneration(worldTransform.scale_);

	//回転軸合成行列を宣言
	Matrix4 matrotate = matRotateGeneration(worldTransform.rotation_);

	//移動するための行列を用意
	Matrix4 matMove = matMoveGeneration(worldTransform.translation_);

	worldTransform.matWorld_ = MathUtility::Matrix4Identity();

	matrotate *= matMove;

	matScale *= matrotate;

	worldTransform.matWorld_ *= matScale;

	worldTransform.TransferMatrix();

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
	matWorldGeneration(worldTransform_);
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