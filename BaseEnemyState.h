#pragma once
#include "Enemy.h"

class Enemy;

class BaseEnemyState
{

public:

	

	Enemy* getEnemy();
	void SetEnemy(Enemy* enemy);
	
	//�������z�֐�
	virtual void Update()=0;

	

private:

	Enemy* enemy_;


};
