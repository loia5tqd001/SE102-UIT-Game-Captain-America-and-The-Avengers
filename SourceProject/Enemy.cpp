#include "pch.h"
#include "Enemy.h"

Enemy::Enemy(Behaviors behavior, const Data& behaviorData, State beforeExplode, int health, Vector2 spawnPos, Grid* grid, State initState) :
	VisibleObject(beforeExplode, spawnPos),
	beforeExplode(beforeExplode),
	health(health),
	grid(grid),
	behavior(behavior),
	behaviorData(behaviorData)
{
	animations.emplace(State::Explode, Animation(SpriteId::Explode, 0.2f));
	if (initState!=State::NotExist)
		curState = initState;
	//we need handing set for each class to this, some dont explode
}

void Enemy::UpdateAnimation(float dt)
{
	if (isFlashing)
		OnFlashing();

	if (curState!=State::DynamiteNapalm_Intact_Injure||curState==State::DynamiteNapalm_Intact_Injure&&!animations.at(curState).IsDoneCycle())
	{
		animations.at(curState).Update(dt);
	}

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
	if (curState == State::Explode) return{};
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

void Enemy::HandleCollisions(float dt, const std::vector<GameObject*>& coObjects)
{
	auto coEvents = CollisionDetector::CalcPotentialCollisions(*this, coObjects, dt);
	if (coEvents.size() == 0)
	{ 
		pos.x += vel.x*dt;
		pos.y += vel.y*dt;
		return; 
	}

	float min_tx, min_ty, nx, ny;
	CollisionDetector::FilterCollisionEvents(coEvents, min_tx, min_ty, nx, ny);

	if (coEvents.size() == 0) return;


	for (auto&e : coEvents)
	{
		if (auto block = dynamic_cast<Block*>(e.pCoObj))
		{
			switch (block->GetType())
			{
				case ClassId::PassableLedge:
					pos.x += vel.x * dt * (1.0f - e.t);
					break;

				case ClassId::RigidBlock:
					pos.x += vel.x * dt * (1.0f - e.t);
					break;

				default:
					AssertUnreachable();
			}
		}
	}
}
