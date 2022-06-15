#include "EnemyStateApproach.h"
#include "EnemyStateLeave.h"

EnemyStateApproach::EnemyStateApproach(Enemy* enemy)
{
	BaseEnemyState::SetEnemy(enemy);
	enemy->SetFireTime(enemy->kFireInterval);
}

void EnemyStateApproach::Update()
{
	//�ړ�(�x�N�g�������Z)
	 getEnemy()->MoveTranslation({0,0,-1});

	 //�e�̔��ˊԊu�����炷
	 getEnemy()->SetFireTime(getEnemy()->GetFireTime() - 1);

	//�K��̈ʒu�ɓ��B�����痣�E
	if (getEnemy()->GetPos().z < 0.0f)
	{
		getEnemy()->ChangeState(new EnemyStateLeave(getEnemy()));
	}
	
	

}