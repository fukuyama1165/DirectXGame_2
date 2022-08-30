#include "Player.h"
#include <cassert>

void Player::Initialize(Model* model, uint32_t textureHandle)
{
	//NULLポインタチェック
	assert(model);

	model_ = model;
	textureHandle_ = textureHandle;

	CameraMat_ = MathUtility::Matrix4Identity();

	//シングルトンインスタンスを取得する
	input_ = Input::GetInstance();
	debugText_ = DebugText::GetInstance();

	//ワールド変換の初期化
	worldTransform_.Initialize();

	worldTransform_.translation_ = { 0,0,50 };

	//3Dレティクルのワールドトランスフォーム初期化
	worldTransform3DReticle_.Initialize();

	//レティクル用テクスチャ取得
	uint32_t textureReticle = TextureManager::Load("Reticle.png");

	//スプライト生成
	sprite2DReticle_.reset(Sprite::Create(textureReticle, { 640,360 }, { 1,1,1,1 }, { 0.5f,0.5f }));

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

	//デスフラグの立った弾を削除(remove_if->条件一致を全て削除)
	bullets_.remove_if([](std::unique_ptr<PlayerBullet>& bullet)//ifの中で簡易的な関数を生成してる->[](引数)
	{
		return bullet->IsDead();
	});

	//キャラクターの移動ベクトル
	Vector3 move = { 0,0,0 };

	const float MoveSpeed = 1;

	XINPUT_STATE joyState;

	if (Input::GetInstance()->GetJoystickState(0, joyState))
	{
		move.x += (float)joyState.Gamepad.sThumbLX / SHRT_MAX * MoveSpeed;
		move.y += (float)joyState.Gamepad.sThumbLY / SHRT_MAX * MoveSpeed;
		
	}

	/*if (input_->PushKey(DIK_W))
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
	}*/

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

	worldTransform_.matWorld_ *= CameraMat_;

	worldTransform_.TransferMatrix();

	Reticle2DMouseAttack(viewProjection);

	//Spaceキーで弾を生成してplayerの正面に進ませる
	Attack();

	//生成されている弾を更新する
	for (std::unique_ptr<PlayerBullet>& bullet : bullets_)
	{
		bullet->Update();
	}


	//自機のワールド座標から3Dレティクルのワールド座標を計算
	{

		////自機から3Dレティクルへの距離
		//const float kDistancePlayerTo3DReticle = 50.0f;

		////自機から3Dレティクルへのオフセット(Z+向き)
		//Vector3 offset = { 0,0,1.0f };

		////自機のワールド行列の回転を反映
		//offset = VectorMat(offset,worldTransform_.matWorld_);

		//offset = offset.normalize() * kDistancePlayerTo3DReticle;

		////3Dレティクルの座標を設定
		//worldTransform3DReticle_.translation_ = { GetWorldPosition().x + offset.x,GetWorldPosition().y + offset.y,GetWorldPosition().z + offset.z };

		//worldTransform3DReticle_.matWorldGeneration();

	}

	//3Dレティクルのワールド座標から2Dレティクルのスクリーン座標を計算
	{
		
		//Vector3 positionReticle = worldTransform3DReticle_.matWorldGetPos();

		//Matrix4 matViewport = {
		//	1280 / 2,0,0,0,
		//	0,-720 / 2,0,0,
		//	0,0,1,0,
		//	1280 / 2 + 0,720 / 2 + 0,0,1
		//};

		////ビュー行列とプロジェクション行列、ビューポート行列を合成する
		//Matrix4 matViewProjectionViewport = viewProjection_.matView * viewProjection_.matProjection* matViewport;

		////ワールド→スクリーン座標変換(ここで3Dから2Dになる)
		//positionReticle = matViewport.VectorMatDivW(matViewProjectionViewport, positionReticle);

		////positionReticle = matViewport.PosMat(positionReticle, matViewport);
		////positionReticle = matViewport.PosMat(worldTransform3DReticle_.matWorldGetPos(), matViewport);

		//
		////スプライトのレティクルに座標設定
		//sprite2DReticle_->SetPosition(Vector2(positionReticle.x, positionReticle.y));
		//debugText_->SetPos(50, 130);
		//debugText_->Printf("repos:(%f,%f,%f)", positionReticle.x, positionReticle.y, positionReticle.z);

	}
	

	debugText_->SetPos(50, 70);
	debugText_->Printf("pos:(%f,%f,%f)",worldTransform_.matWorldGetPos().x, worldTransform_.matWorldGetPos().y,worldTransform_.matWorldGetPos().z);
	
}

void Player::Draw(ViewProjection& viewProjection)
{

	//3Dモデルを描画
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
	model_->Draw(worldTransform3DReticle_, viewProjection);

	for(std::unique_ptr<PlayerBullet>& bullet:bullets_)
	{
		bullet->Draw(viewProjection);
	}

}

void Player::DrawUI()
{
	sprite2DReticle_->Draw();
}

void Player::Rotate()
{
	//キャラクターの移動ベクトル
	Vector3 rotate = { 0,0,0 };

	const float MoveSpeed = 0.1f;

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
	//if (input_->TriggerKey(DIK_SPACE))
	//{
	//	//発射地点の為に自キャラの座標をコピー
	//	Vector3 position = worldTransform_.matWorldGetPos();

	//	//移動量を追加
	//	const float kBulletSpeed = 1.0f;
	//	Vector3 velocity(0, 0, 0);
	//	velocity = worldTransform3DReticle_.matWorldGetPos() - worldTransform_.matWorldGetPos();
	//	velocity = velocity.normalize() * kBulletSpeed;

	//	//速度ベクトルを自機の向きに合わせて回転する
	//	velocity = VectorMat(velocity, worldTransform_.matWorld_);

	//	//弾の生成と初期化
	//	std::unique_ptr<PlayerBullet> newBullet = std::make_unique<PlayerBullet>();
	//	newBullet->Initlize(model_, position,velocity);

	//	//弾を登録
	//	bullets_.push_back(std::move(newBullet));
	//}

	XINPUT_STATE joyState;

	//ゲームパッド未接続なら何もせず抜ける
	if (!Input::GetInstance()->GetJoystickState(0, joyState))
	{
		return;
	}

	//Rトリガーを押していたら
	if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER)
	{
		//発射地点の為に自キャラの座標をコピー
		Vector3 position = worldTransform_.matWorldGetPos();

		//移動量を追加
		const float kBulletSpeed = 1.0f;
		Vector3 velocity(0, 0, 0);
		velocity = worldTransform3DReticle_.matWorldGetPos() - worldTransform_.matWorldGetPos();
		velocity = velocity.normalize() * kBulletSpeed;

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

void Player::SetCameraMat(Matrix4 CameraMat)
{
	CameraMat_ = CameraMat;
}

void Player::Reticle2DMouseAttack(ViewProjection viewProjection)
{

	POINT mousePosition={};

	////スクリーン座標を取得
	//GetCursorPos(&mousePosition);

	////クライアントエリア座標に変換する
	//HWND hwnd = WinApp::GetInstance()->GetHwnd();
	//ScreenToClient(hwnd, &mousePosition);

	//sprite2DReticle_->SetPosition(Vector2(mousePosition.x, mousePosition.y));

	Vector2 spritePosition = sprite2DReticle_->GetPosition();

	XINPUT_STATE joyState;

	if (Input::GetInstance()->GetJoystickState(0, joyState))
	{
		spritePosition.x += (float)joyState.Gamepad.sThumbRX / SHRT_MAX * 7.0f;
		spritePosition.y -= (float)joyState.Gamepad.sThumbRY / SHRT_MAX * 7.0f;

		sprite2DReticle_->SetPosition(spritePosition);
	}



	Matrix4 matViewport = {
			640,0,0,0,
			0,-360,0,0,
			0,0,1,0,
			640 + 0,360 + 0,0,1
	};

	//ビュー行列とプロジェクション行列、ビューポート行列を合成する
	Matrix4 matVPV;
	matVPV = viewProjection.matView;
	matVPV *= viewProjection.matProjection;
	matVPV *= matViewport;
	//合成行列の逆行列を計算する
	Matrix4 matInverseVPV = MathUtility::Matrix4Inverse(matVPV);
	
	//スクリーン座標
	Vector3 posNear = Vector3(sprite2DReticle_->GetPosition().x, sprite2DReticle_->GetPosition().y, 0);
	Vector3 posFar = Vector3(sprite2DReticle_->GetPosition().x, sprite2DReticle_->GetPosition().y, 1);

	//スクリーン座標系からワールド座標系へ
	posNear = VectorMatDivW(matInverseVPV, posNear);
	posFar = VectorMatDivW(matInverseVPV, posFar);

	//マウスレイの方向
 	Vector3 mouseDirection = nainavec3(posFar,posNear);
	mouseDirection = mouseDirection.normalize();

	//カメラから照準オブジェクトの距離
	const float kDistanceTestObject = 100;

	Vector3 A = posNear;
	A += Vector3(mouseDirection.x * kDistanceTestObject, mouseDirection.y * kDistanceTestObject, mouseDirection.z * kDistanceTestObject);
	worldTransform3DReticle_.translation_ = A;

	/*worldTransform3DReticle_.translation_ = posNear+mouseDirection * kDistanceTestObject;*/
	
	worldTransform3DReticle_.matWorldGeneration();

	debugText_->SetPos(20, 200);
	debugText_->Printf("mouse ScreenPos:(%d,%d)", mousePosition.x, mousePosition.y);
	debugText_->SetPos(20, 220);
	debugText_->Printf("Near:(%f,%f,%f)", posNear.x, posNear.y, posNear.z);
	debugText_->SetPos(20, 240);
	debugText_->Printf("Far:(%f,%f,%f)", posFar.x, posFar.y, posFar.z);
	debugText_->SetPos(20, 260);
	debugText_->Printf("mouseObject:(%f,%f,%f)", worldTransform3DReticle_.translation_.x, worldTransform3DReticle_.translation_.y, worldTransform3DReticle_.translation_.z);


}