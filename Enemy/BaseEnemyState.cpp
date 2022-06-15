#include "BaseEnemyState.h"



Enemy* BaseEnemyState::getEnemy()
{
	return enemy_;
}

void BaseEnemyState::SetEnemy(Enemy* enemy)
{
	enemy_ = enemy;
}