#pragma once
#include "BaseEnemyState.h"

class EnemyStateLeave :public BaseEnemyState
{
public:

	EnemyStateLeave(Enemy* enemy);

	void Update(Vector3 speed);


};