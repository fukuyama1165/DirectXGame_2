#pragma once
#include "Enemy.h"

class Enemy;

class BaseEnemyState
{

public:

	

	Enemy* getEnemy();
	void SetEnemy(Enemy* enemy);
	
	//ƒˆ‰¼‘zŠÖ”
	virtual void Update()=0;

	

private:

	Enemy* enemy_;


};
