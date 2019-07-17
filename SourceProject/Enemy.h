#pragma once

class Enemy : public VisibleObject
{
protected:
	int health;
	Grid* grid;
	float FALL_BACK = 50.0f;
	State beforeExplode = State::Destroyed;

	void UpdateAnimation(float dt);

public:
	Enemy(State state, int health, Vector2 spawnPos, Vector2 vel, int nx, Grid* grid);

	void TakeDamage(int damage);
};

