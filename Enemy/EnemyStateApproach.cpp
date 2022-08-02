#include "EnemyStateApproach.h"
#include "EnemyStateLeave.h"

EnemyStateApproach::EnemyStateApproach(Enemy* enemy)
{
	BaseEnemyState::SetEnemy(enemy);
}

void EnemyStateApproach::Update(Vector3 speed)
{
	//ˆÚ“®(ƒxƒNƒgƒ‹‚ð‰ÁŽZ)
	 getEnemy()->MoveTranslation(speed);

	//‹K’è‚ÌˆÊ’u‚É“ž’B‚µ‚½‚ç—£’E
	if (getEnemy()->GetPos().z < 0.0f)
	{
		//’e‚ð”­ŽË‚·‚é‚Ì‚à‚â‚ß‚é
		getEnemy()->FireTimeReMoved();
		getEnemy()->ChangeState(new EnemyStateLeave(getEnemy()));
	}
	
	

}