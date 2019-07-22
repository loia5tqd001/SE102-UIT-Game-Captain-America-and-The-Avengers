#pragma once
#include "Bullet.h"
class BulletEnemyFly :
	public Bullet
{
public:
	BulletEnemyFly(int nx, int type, Enemy *enemy, const Vector2 & spawnPos, Vector2 vel);

};

