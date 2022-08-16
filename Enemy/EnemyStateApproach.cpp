#include "EnemyStateApproach.h"
#include "EnemyStateLeave.h"

EnemyStateApproach::EnemyStateApproach(Enemy* enemy)
{
	BaseEnemyState::SetEnemy(enemy);
}

void EnemyStateApproach::Update(Vector3 speed)
{
	//�ړ�(�x�N�g�������Z)
	 getEnemy()->MoveTranslation(speed);

	//�K��̈ʒu�ɓ��B�����痣�E
	if (getEnemy()->GetPos().z < 0.0f)
	{
		//�e�𔭎˂���̂���߂�
		getEnemy()->FireTimeReMoved();
		getEnemy()->ChangeState(new EnemyStateLeave(getEnemy()));
	}
	
	

}