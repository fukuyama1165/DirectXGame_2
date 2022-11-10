#include "Player.h"
#include <cassert>

void Player::Initialize(Model* model, Model* playerModel, uint32_t textureHandle,RailCamera* camera)
{
	//NULLポインタチェック
	assert(model);
	assert(playerModel);

	model_ = model;
	playerModel_ = playerModel;
	textureHandle_ = textureHandle;
	this->camera=camera;


	//シングルトンインスタンスを取得する
	input_ = Input::GetInstance();
	debugText_ = DebugText::GetInstance();

	//ワールド変換の初期化
	worldTransform_.Initialize();

	target.Initialize();

	worldTransform_.translation_ = { 0,0,0 };

	moveVec = { 0,0,0 };

	viewProjection_.Initialize();

	move_speed = 0.3f;

	hopper_dash = false;

	cooltime = 0;

	yuyotime = 0;

	hopper_speed = 0;

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

	Vector3 up = { 0,1.0f,0 };

	moveVec = { 0,0,0 };

	Vector3 rota = { 0,0,0 };

	Vector3 Flont = camera->getForwardVec();

	Flont.y = 0;

	//Flont = worldTransform_.matWorld_.VectorMat(Flont, worldTransform_.matWorld_);
	Flont.normalize();


	bullets_.remove_if([](std::unique_ptr<PlayerBullet>& bullet)
		{
			return bullet->IsDead();
		});


	//移動
	if (!hopper_dash)
	{
		if (input_->PushKey(DIK_UP))
		{
			worldTransform_.translation_ += Flont * move_speed;
			moveVec.z = 1;
		}
		if (input_->PushKey(DIK_DOWN))
		{
			worldTransform_.translation_ -= Flont * move_speed;
			moveVec.z = -1;
		}
		if (input_->PushKey(DIK_RIGHT))
		{
			worldTransform_.translation_ -= Vector3(-Flont.z,0,Flont.x) * move_speed;
			moveVec.x = 1;
		}
		if (input_->PushKey(DIK_LEFT))
		{
			worldTransform_.translation_ += Vector3(-Flont.z, 0, Flont.x) * move_speed;
			moveVec.x = -1;

		}
	}
	
	float p_pos = atan2(moveVec.x, moveVec.z);
	float c_vec = atan2(Flont.x, Flont.z);

	worldTransform_.rotation_.y = (p_pos + c_vec);

	Vector3 mae = { 0,0,1.0f };

	mae = worldTransform_.matWorld_.VectorMat(mae, worldTransform_.matWorld_);

	mae.normalize();

	target.translation_ = worldTransform_.translation_ + (mae * 5.0f);

	

	worldTransform_.translation_.y -= 0.1f;
	moveVec.y = -1;




	if (input_->PushKey(DIK_SPACE))
	{
		Attack(camera->getForwardVec());
	}

	for (std::unique_ptr<PlayerBullet>& bullet : bullets_)
	{
		bullet->Update();
	}

	
	target.matWorldGeneration();


	worldTransform_.matWorldGeneration();
		
}

void Player::Draw(ViewProjection& viewProjection)
{

	//3Dモデルを描画
	playerModel_->Draw(worldTransform_, viewProjection);
	model_->Draw(target, viewProjection);
	//model_->Draw(worldTransform3DReticle_, viewProjection);

	for(std::unique_ptr<PlayerBullet>& bullet:bullets_)
	{
		bullet->Draw(viewProjection);
	}

}





void Player::Attack(Vector3 flont)
{
	
	const float kBulletSpeed = 10.0f;

	Vector3 velocity=flont;

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

Vector3 Player::PlayerMoveVec()
{
	return moveVec;
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

WorldTransform Player::GetMat()
{
	return worldTransform_;
}

void Player::SetWorldPosition(Vector3 osimodosi)
{
	worldTransform_.translation_ = osimodosi;
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