#include "Player.h"
#include <cassert>

void Player::Initialize(RailCamera* camera, bosstest* boss)
{
	//NULLポインタチェック
	
	assert(boss);

	
	this->camera=camera;
	this->boss = boss;


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

	hopper_count = 0;

	cooldown = false;

	B_bottan = false;
	old_B_bottan = false;

	latetime = 0;

	height = WinApp::GetInstance()->kWindowHeight;
	width = WinApp::GetInstance()->kWindowWidth;


}

void Player::ResourceInitialize(Model* model, Model* playerModel, uint32_t textureHandle, uint32_t Reticletexture)
{
	assert(model);
	assert(playerModel);

	model_ = model;
	playerModel_ = playerModel;
	textureHandle_ = textureHandle;

	Reticle.reset(Sprite::Create(Reticletexture, Vector2(width/2, height/ 2), Vector4(1, 1, 1, 1), Vector2(0.5f, 0.5f)));
	bosstarget.reset(Sprite::Create(textureHandle,Vector2(0,0), Vector4(1, 1, 1, 1), Vector2(0.5f, 0.5f)));
}

Player::Player()
{
}

Player::~Player()
{
}

void Player::Update(ViewProjection viewProjection)
{
	XINPUT_STATE joystate;

	moveVec = { 0,0,0 };

	Vector3 Flont = camera->getForwardVec();

	Flont.y = 0;

	Flont.normalize();

	bullets_.remove_if([](std::unique_ptr<PlayerBullet>& bullet)
		{
			return bullet->IsDead();
		});

	if (Input::GetInstance()->GetJoystickState(0, joystate))
	{
		moveVec.x += (float)joystate.Gamepad.sThumbLX / SHRT_MAX;
		moveVec.z += (float)joystate.Gamepad.sThumbLY / SHRT_MAX;
	}


	if (input_->PushKey(DIK_UP))
	{
		moveVec.z = 1;
	}
	if (input_->PushKey(DIK_DOWN))
	{
		moveVec.z = -1;
	}
	if (input_->PushKey(DIK_RIGHT))
	{
		moveVec.x = 1;
	}
	if (input_->PushKey(DIK_LEFT))
	{
		moveVec.x = -1;
	}

	//移動
	
	
	float p_pos = atan2(moveVec.x, moveVec.z);
	float c_vec = atan2(Flont.x, Flont.z);

	worldTransform_.rotation_.y = (p_pos + c_vec);

	Vector3 mae = { 0,0,1.0f };

	mae = worldTransform_.matWorld_.VectorMat(mae, worldTransform_.matWorld_);

	mae.normalize();

	target.translation_ = worldTransform_.translation_ + (mae * 5.0f);

	if (!hopper_dash && !cooldown)
	{
		if(moveVec.x!=0|| moveVec.z != 0)
		worldTransform_.translation_ += mae * move_speed;
	}

	old_B_bottan = B_bottan;

	if (joystate.Gamepad.wButtons&XINPUT_GAMEPAD_A)
	{
		B_bottan = true;
	}
	else
	{
		B_bottan = false;
	}
	

	if ((input_->TriggerKey(DIK_Z)||(B_bottan&&!old_B_bottan))&& hopper_speed <= 0&&!cooldown&&hopper_count<hopper_limit)
	{
		hopper_dash = true;

		hopper_count++;

		cooltime += 30;

		yuyotime = 120;

		hopper_speed = 10.0f;

		dash_vec = mae;

		dash_vec.normalize();

	}
	if (cooltime > 150)
	{
		cooltime = 150;
	}

	if (hopper_dash)
	{
		if (hopper_speed >= 0)
		{
			worldTransform_.translation_ += dash_vec * hopper_speed;
			hopper_speed -= 1.0f;
		}

		if (hopper_speed < 0)
		{
			yuyotime--;
		}
		if (yuyotime <= 0)
		{
			cooltime--;
			cooldown = true;
		}
		if (cooltime <= 0)
		{
			hopper_dash = false;
			cooldown = false;
			hopper_count = 0;
		}

	}
	

	worldTransform_.translation_.y -= 0.1f;
	moveVec.y = -1;




	if ((input_->PushKey(DIK_SPACE)||(joystate.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER))&& hopper_speed <= 0)
	{
		if (latetime <= 0)
		{
			Attack(camera->getForwardVec());
			latetime = firelate;
		}
		latetime--;
	}

	for (std::unique_ptr<PlayerBullet>& bullet : bullets_)
	{
		bullet->Update();
	}

	
	target.matWorldGeneration();


	worldTransform_.matWorldGeneration();

	bosstarget->SetPosition(kasu(boss->getPos()));

	
		
}

void Player::Draw(ViewProjection& viewProjection)
{

	//3Dモデルを描画
	playerModel_->Draw(worldTransform_, viewProjection);
	//model_->Draw(target, viewProjection);
	//model_->Draw(worldTransform3DReticle_, viewProjection);

	for(std::unique_ptr<PlayerBullet>& bullet:bullets_)
	{
		bullet->Draw(viewProjection);
	}

}

void Player::DrawUI()
{
	Reticle->Draw();
	if (LockOn())
	{
		//bosstarget->Draw();
	}
}

Vector2 Player::kasu(WorldTransform obj)
{
	Vector3 positionReticle = obj.matWorldGetPos();

	Matrix4 matViewport = {
	    1280 / 2,0,0,0,
	    0,-720 / 2,0,0,
	    0,0,1,0,
	    1280 / 2 + 0,720 / 2 + 0,0,1
	};

	//ビュー行列とプロジェクション行列、ビューポート行列を合成する
	Matrix4 matViewProjectionViewport = camera->getView().matView;
	matViewProjectionViewport *= camera->getView().matProjection;
	matViewProjectionViewport *= matViewport;

	//ワールド→スクリーン座標変換(ここで3Dから2Dになる)
	positionReticle = matViewport.VectorMatDivW(matViewProjectionViewport, positionReticle);
	
	//スプライトのレティクルに座標設定
	return Vector2(positionReticle.x, positionReticle.y);
}





void Player::Attack(Vector3 flont)
{
	
	const float kBulletSpeed = 5.0f;
	Vector3 velocity;
	if (LockOn())
	{
		velocity = boss->GetWorldPosition() - camera->GetWorldPosition();
	}
	else
	{
		velocity = flont;

	}
	//velocity = worldTransform_.matWorld_.VectorMat(velocity,worldTransform_.matWorld_);
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

bool Player::LockOn()
{
	if ((width / 2) - 120.0f < bosstarget->GetPosition().x && (width / 2) + 120.0f > bosstarget->GetPosition().x && (height / 2) - 120.0f < bosstarget->GetPosition().y && (height / 2) + 120.0f > bosstarget->GetPosition().y)
	{
		return true;
	}
	else
	{
		return false;

	}
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