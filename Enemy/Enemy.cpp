#include "Enemy.h"
#include "EnemyStateApproach.h"
#include <cassert>

Enemy::Enemy()
{
}

Enemy::~Enemy()
{
	delete state_;
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

	//離脱する時のスピードをセット
	LeaveVelocity_ = { 0,0.1f,0.1f };

	state_ = new EnemyStateApproach(getThis());

}

void Enemy::Update()
{
	//速度を足して移動する
	//worldTransform_.translation_ += Velocity_;

	/*switch (phase_)
	{
	case Enemy::Phase::Approach:

		

		break;
	case Enemy::Phase::Leave:

		
		break;
	default:
		break;
	}*/

	state_->Update();

	//(this->*PhaseMoveP[static_cast<size_t>(phase_)])();

	//行列の更新
	worldTransform_.matWorldGeneration();

	//デスフラグの立った弾を削除(remove_if->条件一致を全て削除)
	bullets_.remove_if([](std::unique_ptr<EnemyBullet>& bullet)//ifの中で簡易的な関数を生成してる->[](引数)
	{
		return bullet->IsDead();
	});

	//if (FireTime_ == 0)
	//{
	//	//発射関数
	//	Fire();

	//	FireTime_ = kFireInterval;
	//}

	timedCalls_.remove_if([](std::unique_ptr<TimeCall>& timeCall)
	{
		return timeCall->IsFinished();
	});

	for (std::unique_ptr<TimeCall>& timeCall : timedCalls_)
	{
		timeCall->Updata();
	}

	//弾の更新処理
	for (std::unique_ptr<EnemyBullet>& bullet : bullets_)
	{
		bullet->Update();
	}

	

}

void Enemy::Draw(const ViewProjection& viewProjection)
{
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
	debugText_->SetPos(50, 90);
	debugText_->Printf("Enemypos:(%f,%f,%f)", worldTransform_.translation_.x, worldTransform_.translation_.y, worldTransform_.translation_.z);
	
	

	for (std::unique_ptr<EnemyBullet>& bullet : bullets_)
	{
		bullet->Draw(viewProjection);
	}

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
	//発射地点の為にキャラの座標をコピー
	Vector3 position = worldTransform_.translation_;

	//移動量を追加
	const float kBulletSpeed = -2.0f;
	Vector3 velocity(0, 0, kBulletSpeed);


	//弾の生成と初期化
	std::unique_ptr<EnemyBullet> newBullet = std::make_unique<EnemyBullet>();
	newBullet->Initlize(model_, position, velocity);

	//弾を登録
	bullets_.push_back(std::move(newBullet));
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
