#include "pch.h"
#include "EnemyRocket.h"
#include "Captain.h"
#include "BulletEnemyRocket.h"


EnemyRocket::EnemyRocket(Behaviors behavior, const Data& behaviorData, Vector2 spawnPos, Grid* grid) :
	Enemy(behavior, std::move(behaviorData), State::EnemyRocket_BeforeExplode, 2, spawnPos, grid)
{
	animations.emplace(State::EnemyRocket_Stand, Animation(SpriteId::EnemyRocket_Stand, 0.1f));
	animations.emplace(State::EnemyRocket_Sitting, Animation(SpriteId::EnemyRocket_Sitting, 0.1f));
	animations.emplace(State::EnemyRocket_Walking, Animation(SpriteId::EnemyRocket_Walking, 0.1f));
	animations.emplace(State::EnemyRocket_BeforeExplode, Animation(SpriteId::EnemyRocket_BeforeExplode, 0.2f));
	animations.emplace(State::Explode, Animation(SpriteId::Explode, 0.25f));

	beforeExplode = State::EnemyRocket_BeforeExplode;
	Explode = State::Explode;
}

void EnemyRocket::SetState(State state)
{
	VisibleObject::SetState(state); 

	switch (state)
	{
	case State::EnemyRocket_Stand:
		vel.x = 0.0f;
		break;
	case State::EnemyRocket_Walking:
		vel.x = nx * WALKING_SPEED;
		break;
	case State::EnemyRocket_Sitting:
		vel.x = 0.0f;
		break;
	case State::EnemyRocket_BeforeExplode:
		vel.x = vel.x = -nx * FALL_BACK;
		break;
	case State::Explode:
		vel.x = 0;
		break;
	}
}

void EnemyRocket::SpawnRocket(float cycle)
{
	static Counter curCounter;
	if (isFlashing) return;
	if (curCounter.CanExcuteCommand(cycle, false))
	{
		grid->SpawnObject(std::make_unique<BulletEnemyRocket>(nx, 1, pos));
	}
}

void EnemyRocket::Update(float dt, const std::vector<GameObject*>& coObjects)
{
	//regular update
	pos.x += vel.x*dt;
	pos.y += vel.y*dt;
	//take damamge

	//}

	// spawn bullet every 0.4s
	//SpawnRocket(4.0f);
	//NOTE:open comment when grid is done


	// update animations
	UpdateAnimation(dt);
}
