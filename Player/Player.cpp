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

	nannka[0] = { 0.5f,1,0 };
	nannka[1] = { -0.5f,1,0 };
	nannka[2] = { 1,0,0 };
	nannka[3] = { -1,0,0 };
	
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

	hozon = { 0,0,0 };



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
	
	
	
	if (LockOn(boss))
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

	if (LockOn(boss))
	{
		Vector3 PtoB = BitVec - worldTransform_.translation_;
		Vector3 kari = PtoB;
		kari.y = 0;
		float kariy = -PtoB.y;
		PtoB.normalize();
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

		for (size_t i = 0; i < gunbitnum; i++)
		{
			Vector3 bitpos = nannka[i];
			bitpos = kyozou.matWorld_.VectorMat(bitpos, kyozou.matWorld_);
			bitpos.normalize();
			target[i].translation_ = easeOutSineVec3(target[i].translation_, worldTransform_.translation_ + (bitpos * 2.0f), timer / bitmovetimer);
		}

		
	}
	else
	{
		for (size_t i = 0; i < gunbitnum; i++)
		{
			Vector3 bitpos = nannka[i];
			bitpos = worldTransform_.matWorld_.VectorMat(bitpos, worldTransform_.matWorld_);
			bitpos.normalize();
			target[i].translation_ = worldTransform_.translation_ + (bitpos * 2.0f);
		}

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
		if (!LockOn(boss))
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

			for (size_t i = 0; i < gunbitnum; i++)
			{
				Vector3 bitpos = nannka[i];
				bitpos = kyozou.matWorld_.VectorMat(bitpos, kyozou.matWorld_);
				bitpos.normalize();
				target[i].translation_ = easeOutSineVec3(target[i].translation_, worldTransform_.translation_ + (bitpos * 2.0f), NormalTimer / BitNormalAttakTime);
			}

			
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
		if (i == 0 || i == 2)
		{
			target[i].rotation_.z += 0.05f;
		}
		else
		{
			target[i].rotation_.z -= 0.05f;
		}
		target[i].matWorldGeneration();
	}
	
	kyozou.matWorldGeneration();

	worldTransform_.matWorldGeneration();

	//bosstarget->SetPosition(kasu(boss->getPos().translation_));

	
		
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
	if (LockOn(boss))
	{
		bosstarget->Draw();
	}
}

Vector2 Player::kasu(Vector3 obj)
{
	Vector3 positionReticle = obj;

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

bool Player::screenLock(WorldTransform pos)
{
	Matrix4 Pos = pos.matWorld_;
	Pos *= camera->getView().matView;
	Pos *= camera->getView().matProjection;

	float objZ = Pos.m[3][2];

	Vector2 scr_pos = kasu(pos.translation_);

	if ((width / 2) - 120.0f < scr_pos.x && (width / 2) + 120.0f > scr_pos.x && (height / 2) - 120.0f < scr_pos.y && (height / 2) + 120.0f > scr_pos.y && objZ > 0)
	{
		return true;
	}

	else
	{
		return false;
	}
	
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
		newBullet->Initlize(model_,target[i].translation_, target[i].rotation_, velocity);

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

bool Player::LockOn(bosstest* obj)
{
	const std::vector<bossHand*>& bosshands = boss->getHand();
	for (int i = 0; i < bosshands.size(); i++)
	{
		if (screenLock(obj->getPos())&&screenLock(bosshands[i]->GetwroldTransform()))
		{
			if (bosshands[i]->getisAttackFlag())
			{
				Vector3 hozon2 = Hikaku(obj->GetWorldPosition(), bosshands[i]->GetwroldTransform().translation_, hozon);
				bosstarget->SetPosition(kasu(hozon2));
				BitVec = hozon2;
				hozon = hozon2;
			}
			else
			{
				bosstarget->SetPosition(kasu(obj->GetWorldPosition()));
				BitVec = obj->GetWorldPosition();
			}


			return true;
		}
		else if(screenLock(obj->getPos()))
		{

			bosstarget->SetPosition(kasu(obj->GetWorldPosition()));
			BitVec = obj->GetWorldPosition();
			return true;
		}
		else if (screenLock(bosshands[i]->GetwroldTransform()))
		{
			if (bosshands[i]->getisAttackFlag())
			{
				Vector3 hozon2 = Hikaku2(bosshands[i]->GetwroldTransform().translation_, hozon);
				bosstarget->SetPosition(kasu(hozon2));
				hozon = hozon2;
				BitVec = hozon2;
			}
			return true;
		}
		else
		{
			hozon = { 0,0,0 };
			return false;

		}
	}

	
}

void Player::SetWorldPosition(Vector3 osimodosi)
{
	worldTransform_.translation_ = osimodosi;
}

Vector3 Player::Hikaku(Vector3 boss, Vector3 hand1, Vector3 hand2)
{
	Vector3 kariA, kariB, kariC;
	kariA = boss - worldTransform_.translation_;
	kariB = hand1 - worldTransform_.translation_;
	kariC = hand2 - worldTransform_.translation_;

	if (kariA.length() < kariB.length() && kariA.length() < kariC.length()) return boss;
	else if (kariB.length() < kariA.length() && kariB.length() < kariC.length()) return hand1;
	else if (kariC.length() < kariA.length() && kariC.length() < kariB.length()) return hand2;
	
}

Vector3 Player::Hikaku2(Vector3 hand1, Vector3 hand2)
{
	Vector3 kariB, kariC;
	kariB = hand1 - worldTransform_.translation_;
	kariC = hand2 - worldTransform_.translation_;

	
	if (kariB.length() <= kariC.length()) return hand1;
	else if (kariC.length() < kariB.length()) return hand2;

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