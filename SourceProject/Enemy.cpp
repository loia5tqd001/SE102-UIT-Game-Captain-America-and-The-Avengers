#include "pch.h"
#include "Enemy.h"

Enemy::Enemy(Behaviors behavior, const Data& behaviorData, State beforeExplode, int health, Vector2 spawnPos, Grid* grid) :
	VisibleObject(beforeExplode, spawnPos),
	beforeExplode(beforeExplode),
	health(health),
	grid(grid),
	behavior(behavior),
	behaviorData(behaviorData)
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
	if (health <= 0) {
		SetState(beforeExplode);
	}
	else {
		OnFlashing(true);
	}
}
