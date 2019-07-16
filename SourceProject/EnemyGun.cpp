#include "pch.h"
#include "EnemyGun.h"
#include "Captain.h"
#include "BulletEnemyGun.h"

EnemyGun::EnemyGun(const Vector2 & spawnPos, const Vector2 & vel, int nx, Grid *grid) : 
	Enemy(State::EnemyGun_Stand, 1, spawnPos, vel, nx, grid)
{
	animations.emplace(State::EnemyGun_Stand, Animation(SpriteId::EnemyGun_Stand, 0.1f));
	animations.emplace(State::EnemyGun_Sitting, Animation(SpriteId::EnemyGun_Sitting, 0.1f));
	animations.emplace(State::EnemyGun_TakeDamage, Animation(SpriteId::EnemyGun_TakeDamage, 0.1f));
	animations.emplace(State::EnemyGun_Walking, Animation(SpriteId::EnemyGun_Walking, 0.1f));
	animations.emplace(State::Explode, Animation(SpriteId::Explode, 0.25f));
}

void EnemyGun::Update(float dt, const std::vector<GameObject*>& coObjects)
{
	//counter, if take damage, flasing in 0,5s then change to explode, keep 2 sprite in 0,2 s fit and destroy
	if (curState == State::EnemyGun_TakeDamage) {

		timeTakenDamaged += GameTimer::Dt();
		OnFlasing();
		if (timeTakenDamaged >= 0.5f)
		{
			SetState(State::Explode);
			timeTakenDamaged = 0.0f;
			OnFlasing(false);
		}
	}
	else if (curState == State::Explode)
	{
		timeExploding += GameTimer::Dt();
		if (timeExploding >= 0.4f)
		{
			SetState(State::Destroyed);
			timeExploding = 0.0f;
		}
	}

	// spawn bullet every 4s
	// NOTE: disable for testing SpawnBullet(4.0f);

	// update animations
	animations.at(curState).Update(dt);
}

void EnemyGun::SetState(State state)
{
	const auto oldHeight = GetHeight();
	VisibleObject::SetState(state);
	pos.y += oldHeight - GetHeight();

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
	case State::EnemyGun_TakeDamage:
	    vel.x = -nx * FALL_BACK;
		OnFlasing(true);
		break;
	case State::Explode:
		vel.x = 0;
		break;
	}
}

void EnemyGun::SpawnBullet(float cycle) 
{
	timeSpawnBullet += GameTimer::Dt();
	if (curState == State::EnemyGun_TakeDamage) return;
	if (timeSpawnBullet >= cycle)
	{
		grid->SpawnObject(std::make_unique<BulletEnemyGun>( pos, Vector2{ 100.0f, 0.0f }, nx));
		timeSpawnBullet = 0;
	}
}

void EnemyGun::TakeDamage(int damage)
{
	health -= damage;
	SetState(State::EnemyGun_TakeDamage);
}


