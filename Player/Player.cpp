#include "Player.h"
#include"easing.h"
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

	for (size_t i = 0; i < gunbitnum; i++)
	{
		target[i].Initialize();
		target[i].scale_ = {0.5f,0.5f,2.0f};
	}

	
	kyozou.Initialize();

	worldTransform_.translation_ = { 0,0,0 };

	moveVec = { 0,0,0 };

	viewProjection_.Initialize();

	move_speed = 0.3f;

	hopper_dash = false;

	cooltime = 0;

	timer=0;

	NormalTimer = 0;

	bitmovetimer=60;

	lockmove = false;


	hopper_speed = 5.0f;

	cooldown = false;

	B_bottan = false;
	old_B_bottan = false;

	latetime = 0;

	height = WinApp::GetInstance()->kWindowHeight;
	width = WinApp::GetInstance()->kWindowWidth;

	hoppertimer = 0;

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
	Vector3 KyozouFlont = Flont;
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

	if (!hopper_dash && !cooldown)
	{
		if (moveVec.x != 0 || moveVec.z != 0)
		{
		
			worldTransform_.translation_ += mae * move_speed;
		}
	}
	Vector3 nannka;
	Vector3 PtoB = boss->GetWorldPosition() - worldTransform_.translation_;
	Vector3 kari = PtoB;
	kari.y = 0;
	float kariy = -PtoB.y;
	PtoB.normalize();
	
	if (LockOn(boss->getPos()))
	{
		lockmove = true;
	}
	else
	{
		lockmove = false;
		timer = 0;
	}
	
	if (lockmove)
	{
		timer++;

		if (timer >= bitmovetimer)
		{
			timer = bitmovetimer;
		}


	}

	if (LockOn(boss->getPos()))
	{
		float gomi = atan2(PtoB.x, PtoB.z);

		for (size_t i = 0; i < gunbitnum; i++)
		{
			target[i].rotation_.y = easeOutSine(target[i].rotation_.y, gomi, timer / bitmovetimer);
		}
		kyozou.rotation_.y = gomi;
		gomi = atan2(kariy, kari.length());
		for (size_t i = 0; i < gunbitnum; i++)
		{
			target[i].rotation_.x = easeOutSine(target[i].rotation_.x, gomi, timer / bitmovetimer);
		}
		kyozou.rotation_.x = gomi;

		

		nannka = { 0.5f,1,0 };
		nannka = kyozou.matWorld_.VectorMat(nannka, kyozou.matWorld_);
		nannka.normalize();
		target[0].translation_ = easeOutSineVec3(target[0].translation_, worldTransform_.translation_ + (nannka * 2.0f), timer / bitmovetimer);

		nannka = { -0.5f,1,0 };
		nannka = kyozou.matWorld_.VectorMat(nannka, kyozou.matWorld_);
		nannka.normalize();
		target[1].translation_ = easeOutSineVec3(target[1].translation_, worldTransform_.translation_ + (nannka * 2.0f), timer / bitmovetimer);

		nannka = { 1,0,0 };
		nannka = kyozou.matWorld_.VectorMat(nannka, kyozou.matWorld_);
		nannka.normalize();
		target[2].translation_ = easeOutSineVec3(target[2].translation_, worldTransform_.translation_ + (nannka * 2.0f), timer / bitmovetimer);
		nannka = { -1,0,0 };
		nannka = kyozou.matWorld_.VectorMat(nannka, kyozou.matWorld_);
		nannka.normalize();
		target[3].translation_ = easeOutSineVec3(target[3].translation_, worldTransform_.translation_ + (nannka * 2.0f), timer / bitmovetimer);

	}
	else
	{

		nannka = { 0.5f,1,0 };
		nannka = worldTransform_.matWorld_.VectorMat(nannka, worldTransform_.matWorld_);
		nannka.normalize();
		target[0].translation_ = worldTransform_.translation_ + (nannka * 2.0f);

		nannka = { -0.5f,1,0 };
		nannka = worldTransform_.matWorld_.VectorMat(nannka, worldTransform_.matWorld_);
		nannka.normalize();
		target[1].translation_ = worldTransform_.translation_ + (nannka * 2.0f);

		nannka = { 1,0,0 };
		nannka = worldTransform_.matWorld_.VectorMat(nannka, worldTransform_.matWorld_);
		nannka.normalize();
		target[2].translation_ = worldTransform_.translation_ + (nannka * 2.0f);

		nannka = { -1,0,0 };
		nannka = worldTransform_.matWorld_.VectorMat(nannka, worldTransform_.matWorld_);
		nannka.normalize();
		target[3].translation_ = worldTransform_.translation_ + (nannka * 2.0f);

		

		for (size_t i = 0; i < gunbitnum; i++)
		{
			
			target[i].rotation_.y = worldTransform_.rotation_.y;
			target[i].rotation_.x = worldTransform_.rotation_.x;
		}
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
	

	if ((input_->TriggerKey(DIK_Z)||(B_bottan&&!old_B_bottan))&&!cooldown)
	{
		hopper_dash = true;

		cooltime = 60;

		hoppertimer = 0;

		dash_vec = mae;

		dash_vec.normalize();

	}
	if (hopper_dash)
	{
		if (!cooldown)
		{
			worldTransform_.translation_ = easeOutSineVec3(worldTransform_.translation_, worldTransform_.translation_ + dash_vec * hopper_speed, hoppertimer / hoppertime);

		}
		hoppertimer++;
		
		
		

		if (hoppertimer>=hoppertime)
		{
			hoppertimer = hoppertime;
			cooltime--;
			cooldown = true;
			
		}
		if (cooltime <= 0)
		{
			hopper_dash = false;
			cooldown = false;
		}

	}
	

	worldTransform_.translation_.y -= 0.1f;
	moveVec.y = -1;

	


	if ((input_->PushKey(DIK_SPACE)||(joystate.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER))&& hoppertimer >= 0)
	{
		if (!LockOn(boss->getPos()))
		{
			Vector3 kari2 = KyozouFlont;
			kari2.y = 0;
			float kariy2 = -KyozouFlont.y;

			float gomi = atan2(Flont.x, Flont.z);
			for (size_t i = 0; i < gunbitnum; i++)
			{
				target[i].rotation_.y = easeOutSine(target[i].rotation_.y, gomi, NormalTimer / BitNormalAttakTime);
			}
			kyozou.rotation_.y = gomi;
			gomi = atan2(kariy2,kari2.length());
			for (size_t i = 0; i < gunbitnum; i++)
			{
				target[i].rotation_.x = easeOutSine(target[i].rotation_.x, gomi, NormalTimer / BitNormalAttakTime);
			}
			kyozou.rotation_.x = gomi;

			nannka = { 0.5f,1,0 };
			nannka = kyozou.matWorld_.VectorMat(nannka, kyozou.matWorld_);
			nannka.normalize();
			target[0].translation_ = easeOutSineVec3(target[0].translation_, worldTransform_.translation_ + (nannka * 2.0f), NormalTimer / BitNormalAttakTime);

			nannka = { -0.5f,1,0 };
			nannka = kyozou.matWorld_.VectorMat(nannka, kyozou.matWorld_);
			nannka.normalize();
			target[1].translation_ = easeOutSineVec3(target[1].translation_, worldTransform_.translation_ + (nannka * 2.0f), NormalTimer / BitNormalAttakTime);

			nannka = { 1,0,0 };
			nannka = kyozou.matWorld_.VectorMat(nannka, kyozou.matWorld_);
			nannka.normalize();
			target[2].translation_ = easeOutSineVec3(target[2].translation_, worldTransform_.translation_ + (nannka * 2.0f), NormalTimer / BitNormalAttakTime);

			nannka = { -1,0,0 };
			nannka = kyozou.matWorld_.VectorMat(nannka, kyozou.matWorld_);
			nannka.normalize();
			target[3].translation_ = easeOutSineVec3(target[3].translation_, worldTransform_.translation_ + (nannka * 2.0f), NormalTimer / BitNormalAttakTime);

			NormalTimer++;

			if (NormalTimer >= BitNormalAttakTime)
			{
				NormalTimer = BitNormalAttakTime;
			}
		}
		else
		{
			NormalTimer = 0;
		}

		
		if (latetime <= 0)
		{
			Attack(camera->getForwardVec());
			latetime = firelate;
		}
		latetime--;
		
	}
	else
	{
		NormalTimer = 0;
	}
	

	for (std::unique_ptr<PlayerBullet>& bullet : bullets_)
	{
		bullet->Update();
	}

	for (size_t i = 0; i < gunbitnum; i++)
	{
		target[i].matWorldGeneration();
	}
	
	kyozou.matWorldGeneration();

	worldTransform_.matWorldGeneration();

	bosstarget->SetPosition(kasu(boss->getPos()));

	
		
}

void Player::Draw(ViewProjection& viewProjection)
{

	//3Dモデルを描画
	playerModel_->Draw(worldTransform_, viewProjection);

	for (size_t i = 0; i < gunbitnum; i++)
	{
		model_->Draw(target[i], viewProjection);
	}
	
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
	if (LockOn(boss->getPos()))
	{
		bosstarget->Draw();
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
	for (size_t i = 0; i < gunbitnum; i++)
	{
		Vector3 velocity(0,0,1);
		
		velocity = target[i].matWorld_.VectorMat(velocity, target[i].matWorld_);
		velocity.normalize();

		velocity *= kBulletSpeed;

		std::unique_ptr <PlayerBullet> newBullet = std::make_unique<PlayerBullet>();
		newBullet->Initlize(model_,target[i].translation_, velocity);

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

bool Player::LockOn(WorldTransform obj)
{

	Matrix4 pos = obj.matWorld_;


	obj.matWorld_ *= camera->getView().matView;
	obj.matWorld_ *= camera->getView().matProjection;

	float objZ = obj.matWorld_.m[3][2];

	if ((width / 2) - 120.0f < bosstarget->GetPosition().x && (width / 2) + 120.0f > bosstarget->GetPosition().x && (height / 2) - 120.0f < bosstarget->GetPosition().y && (height / 2) + 120.0f > bosstarget->GetPosition().y&&objZ>0)
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