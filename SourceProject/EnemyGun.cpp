#include "pch.h"
#include "EnemyGun.h"
#include "Captain.h"
#include "BulletEnemyGun.h"

EnemyGun::EnemyGun(Behaviors behavior, const Data& behaviorData, Vector2 spawnPos, Grid* grid) : 
	Enemy(behavior, std::move(behaviorData), State::EnemyGun_BeforeExplode, 1, spawnPos, grid)
{
	// switch behavior data, case:
	animations.emplace(State::EnemyGun_Stand, Animation(SpriteId::EnemyGun_Stand, 0.1f));
	animations.emplace(State::EnemyGun_Sitting, Animation(SpriteId::EnemyGun_Sitting, 0.1f));
	animations.emplace(State::EnemyGun_BeforeExplode, Animation(SpriteId::EnemyGun_BeforeExplode, 0.3f));
	animations.emplace(State::EnemyGun_Walking, Animation(SpriteId::EnemyGun_Walking, 0.1f));
	animations.emplace(State::Explode, Animation(SpriteId::Explode, 0.25f));
	
	beforeExplode = State::EnemyGun_BeforeExplode;
	Explode = State::Explode;
}

void EnemyGun::Update(float dt, const std::vector<GameObject*>& coObjects)
{
	pos.x += vel.x*dt;

	// spawn bullet every 4s
	// NOTE: disable for testing SpawnBullet(4.0f);
	//SpawnBullet(4.0f);
	//NOTE:open comment when grid is done

	// update animations
	UpdateAnimation(dt);
}

void EnemyGun::SetState(State state)
{
	VisibleObject::SetState(state);

	switch (state)
	{
	case State::EnemyGun_Stand:
		vel.x = 0.0f;
		break;
	case State::EnemyGun_Walking:
		vel.x = nx * WALKING_SPEED;
		break;
	case State::EnemyGun_Sitting:
		vel.x = 0.0f;
		break;
	case State::EnemyGun_BeforeExplode:
		vel.x = -nx * FALL_BACK;
		break;
	case State::Explode:
		vel.x = 0;
		break;
	}
}

void EnemyGun::SpawnBullet(float cycle) 
{
	timeSpawnBullet += GameTimer::Dt();
	if (isFlashing) return;
	if (timeSpawnBullet >= cycle)
	{
		grid->SpawnObject(std::make_unique<BulletEnemyGun>(nx, pos));
		timeSpawnBullet = 0;
	}
}
