#include "Enemy.h"
#include "EnemyStateApproach.h"
#include "Player.h"
#include "GameScene.h"
#include <cassert>

Enemy::Enemy()
{
}

Enemy::~Enemy()
{
	delete state_;
}

void Enemy::Initialize(Model* model, Model* enemyModel, const Vector3& position, const Vector3& velocity)
{

	//ヌルポチェック
	assert(model);
	assert(enemyModel);

	model_ = model;
	enemyModel_ = enemyModel;

	debugText_ = DebugText::GetInstance();

	//テクスチャハンドルを設定
	textureHandle_= TextureManager::Load("enemyBall.png");

	//ワールドトランスフォームの初期化
	worldTransform_.Initialize();

	//引数で受け取った初期座標をセット
	worldTransform_.translation_ = position;

	//引数で受け取った速度をセット
	Velocity_ = velocity;

	//離脱する時のスピードをセット
	LeaveVelocity_ = { 0,0.1f,0.1f };

	//動きを近づくので初期化する
	state_ = new EnemyStateApproach(getThis());
	
	//弾の発射をスタートする
	FireAndReset();

}

void Enemy::Update()
{
	//現在のstateで更新する
	state_->Update(Velocity_);

	//
	//(this->*PhaseMoveP[static_cast<size_t>(phase_)])();

	//行列の更新
	worldTransform_.matWorldGeneration();

	if (worldTransform_.matWorldGetPos().y > 80)
	{
		IsDead_ = true;
	}

	//if (FireTime_ == 0)
	//{
	//	//発射関数
	//	Fire();

	//	FireTime_ = kFireInterval;
	//}

	//カウントダウンが終了していたら消す
	timedCalls_.remove_if([](std::unique_ptr<TimeCall>& timeCall)
	{
		return timeCall->IsFinished();
	});

	//生成されているカウントダウンを更新
	for (std::unique_ptr<TimeCall>& timeCall : timedCalls_)
	{
		timeCall->Updata();
	}

	

}

void Enemy::Draw(const ViewProjection& viewProjection)
{
	//モデルを描画
	enemyModel_->Draw(worldTransform_, viewProjection);
	/*debugText_->SetPos(50, 90);
	debugText_->Printf("Enemypos:(%f,%f,%f)", worldTransform_.translation_.x, worldTransform_.translation_.y, worldTransform_.translation_.z);*/
	
	
	

}

void Enemy::ApproachMove()
{
	//移動(ベクトルを加算)
	worldTransform_.translation_ += Velocity_;

	//規定の位置に到達したら離脱
	if (worldTransform_.translation_.z < 0.0f)
	{
		phase_ = Phase::Leave;
	}
}

void Enemy::LeaveMove()
{
	//移動(ベクトルを加算)
	worldTransform_.translation_ += LeaveVelocity_;

}

//関数ポインタをセット
void (Enemy::* Enemy::PhaseMoveP[])() =
{
	&Enemy::ApproachMove,
	&Enemy::LeaveMove
};

void Enemy::MoveTranslation(Vector3 Velocity)
{
	worldTransform_.translation_ += Velocity;
}

Vector3	Enemy::GetPos()
{
	return worldTransform_.translation_;
}

void Enemy::ChangeState(BaseEnemyState* newState)
{
	delete state_;
	state_ = newState;
}

Enemy* Enemy::getThis()
{
	return this;
}

void Enemy::Fire()
{
	////発射地点の為にキャラの座標をコピー
	//Vector3 position = worldTransform_.translation_;

	////移動量を追加
	//const float kBulletSpeed = -2.0f;
	//Vector3 velocity(0, 0, kBulletSpeed);

	assert(player_);

	//弾の速度
	const float kBulletSpeed = 1.0f;

	Vector3 playerVec;
	Vector3 enemyVec;
	Vector3 enemyPlayerVec;

	playerVec = player_->GetWorldPosition();
	enemyVec = GetWorldPosition();

	enemyPlayerVec = { playerVec.x - enemyVec.x,playerVec.y - enemyVec.y,playerVec.z - enemyVec.z };

	float len = sqrtf(enemyPlayerVec.x * enemyPlayerVec.x + enemyPlayerVec.y * enemyPlayerVec.y + enemyPlayerVec.z * enemyPlayerVec.z);

	enemyPlayerVec /= len;

	Vector3 bullrtSpeed = { enemyPlayerVec.x * kBulletSpeed,enemyPlayerVec.y * kBulletSpeed,enemyPlayerVec.z * kBulletSpeed };

	//弾の生成と初期化
	std::unique_ptr<EnemyBullet> newBullet = std::make_unique<EnemyBullet>();
	newBullet->Initlize(model_, worldTransform_.translation_, bullrtSpeed);

	//弾を登録
	//bullets_.push_back(std::move(newBullet));

	gameScene_->AddEnemyBullet(std::move(newBullet));

}


void Enemy::SetFireTime(int32_t FireTime)
{
	FireTime_ = FireTime;
}

int32_t Enemy::GetFireTime()
{
	return FireTime_;
}

void Enemy::FireAndReset()
{
	//弾を発射する
	Fire();

	//発射タイマーをセットする
	timedCalls_.push_back(std::make_unique<TimeCall>(std::bind(&Enemy::FireAndReset, this), kFireInterval));

}

void Enemy::FireTimeReMoved()
{
	//強制的に全ての要素を消す
	timedCalls_.remove_if([](std::unique_ptr<TimeCall>& timeCall)
	{
		return 1;
	});
}

Vector3 Enemy::GetWorldPosition()
{
	//ワールド座標を入れる変数
	Vector3 worldpos;

	//ワールド行列の平行移動成分を取得(ワールド座標)
	worldpos.x = worldTransform_.matWorld_.m[3][0];
	worldpos.y = worldTransform_.matWorld_.m[3][1];
	worldpos.z = worldTransform_.matWorld_.m[3][2];

	return worldpos;
}

void Enemy::OnCollision()
{
	HP--;

	if (HP <= 0)
	{
		IsDead_ = true;
	}
}
