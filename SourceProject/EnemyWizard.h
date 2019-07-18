#pragma once
#include "Enemy.h"
class EnemyWizard :
	public Enemy
{
public:
	EnemyWizard(const Vector2& spawnPos, const Vector2& vel, int nx, Grid *grid);
};

