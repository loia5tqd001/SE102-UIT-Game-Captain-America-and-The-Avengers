#pragma once

class Enemy : public VisibleObject
{
protected:
	int health;
	Grid* grid;

public:
	Enemy(State state, int health, Vector2 spawnPos, Vector2 vel, int nx, Grid* grid) :
		VisibleObject(state, spawnPos, vel, nx),
		health(health),
		grid(grid)
	{}

	virtual void TakeDamage(int damage) = 0;
};

