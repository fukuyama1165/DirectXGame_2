#include "EnemyStateLeave.h"

EnemyStateLeave::EnemyStateLeave(Enemy* enemy)
{
	SetEnemy(enemy);
}

void EnemyStateLeave::Update(Vector3 speed)
{

	getEnemy()->MoveTranslation({0,0.1f,0.1f});

}

