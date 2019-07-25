#pragma once
#include "VisibleObject.h"
#include "Enemy.h"

class Bullet : public VisibleObject
{
protected:
	int damage;
	Enemy *enemy; //need host enemy width and pos to flip x
public:
	Bullet(State initState, int damage, Vector2 pos, Vector2 vel, int nx, Enemy *enemy) :
		VisibleObject(initState, pos, vel, nx),
		damage(damage)
	{
		this->enemy = enemy;
	}

	inline int GetDamage() const { return damage; }
	virtual void HitCaptain() {}; //override this if explode
};

