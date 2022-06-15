#include "EnemyStateApproach.h"
#include "EnemyStateLeave.h"

EnemyStateApproach::EnemyStateApproach(Enemy* enemy)
{
	BaseEnemyState::SetEnemy(enemy);
	enemy->SetFireTime(enemy->kFireInterval);
}

void EnemyStateApproach::Update()
{
	//移動(ベクトルを加算)
	 getEnemy()->MoveTranslation({0,0,-1});

	//規定の位置に到達したら離脱
	if (getEnemy()->GetPos().z < 0.0f)
	{
		getEnemy()->FireTimeReMoved();
		getEnemy()->ChangeState(new EnemyStateLeave(getEnemy()));
	}
	
	

}