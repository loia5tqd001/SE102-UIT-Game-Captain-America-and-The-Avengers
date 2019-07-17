#include "pch.h"
#include "Enemy.h"

Enemy::Enemy(State state, int health, Vector2 spawnPos, Vector2 vel, int nx, Grid* grid) :
	VisibleObject(state, spawnPos, vel, nx),
	health(health),
	grid(grid)
{}

void Enemy::UpdateAnimation(float dt)
{
	if (isFlashing)
		OnFlashing();

	if (animations.at(beforeExplode).IsDoneCycle())
	{
		SetState(State::Explode);
	}
	else if (animations.at(State::Explode).IsDoneCycle())
	{
		SetState(State::Destroyed);
	}

	animations.at(curState).Update(dt);
}

void Enemy::TakeDamage(int damage)
{
	assert(damage > 0);
	if (isFlashing) return; // has just being damaged and is flashing, don't be too evil, give me time to recover please

	health -= damage;
	if (health <= 0)
	{
		assert(beforeExplode != State::Destroyed);
		SetState(beforeExplode);
	}
	else
	{
		OnFlashing(true);
	}
}
