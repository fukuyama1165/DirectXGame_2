#include "EnemyStateApproach.h"
#include "EnemyStateLeave.h"

EnemyStateApproach::EnemyStateApproach(Enemy* enemy)
{
	BaseEnemyState::SetEnemy(enemy);
}

void EnemyStateApproach::Update(Vector3 speed)
{
	//移動(ベクトルを加算)
	 getEnemy()->MoveTranslation(speed);

	//規定の位置に到達したら離脱
	if (getEnemy()->GetPos().z < 0.0f)
	{
		//弾を発射するのもやめる
		getEnemy()->FireTimeReMoved();
		getEnemy()->ChangeState(new EnemyStateLeave(getEnemy()));
	}
	
	

}