#include "pch.h"
#include "Enemy.h"

Enemy::Enemy(Behaviors behavior, const Data& behaviorData, State beforeExplode, int health, Vector2 spawnPos, Grid* grid) :
	VisibleObject(beforeExplode, spawnPos),
	beforeExplode(beforeExplode),
	health(health),
	grid(grid),
	behavior(behavior),
	behaviorData(behaviorData)
{
	animations.emplace(State::Explode, Animation(SpriteId::Explode, 0.25f));
	//we need handing set for each class to this, some dont explode
}

void Enemy::UpdateAnimation(float dt)
{
	if (isFlashing)
		OnFlashing();

	animations.at(curState).Update(dt);

	if (animations.at(beforeExplode).IsDoneCycle())
	{
		SetState(Explode);
	}
	else if (animations.at(Explode).IsDoneCycle())
	{
		SetState(State::Destroyed);
	}

}

RectF Enemy::GetBBox() const
{
	if (curState == beforeExplode || curState == State::Explode) return{};
	return VisibleObject::GetBBox();
}

void Enemy::TakeDamage(int damage)
{
	assert(damage > 0);

	health -= damage;
	if (health <= 0) 
	{
		SetState(beforeExplode);
	}
	else {
		OnFlashing(true);
	}
}
