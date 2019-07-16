#pragma once
#include "VisibleObject.h"

class Bullet : public VisibleObject
{
protected:
	int damage;

public:
	Bullet(State initState, int damage, Vector2 pos, Vector2 vel, int nx) :
		VisibleObject(initState, pos, vel, nx),
		damage(damage)
	{}

	inline int GetDamage() const { return damage; }
};

