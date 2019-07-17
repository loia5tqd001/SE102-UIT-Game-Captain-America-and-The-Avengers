#include "pch.h"
#include "EnemyRocket.h"
#include "Captain.h"
#include "BulletEnemyRocket.h"


EnemyRocket::EnemyRocket(const Vector2 & spawnPos, const Vector2 & vel, int nx, Grid * grid) :
	Enemy(State::EnemyRocket_Stand, 30, spawnPos, vel, nx, grid)
{
	animations.emplace(State::EnemyRocket_Stand, Animation(SpriteId::EnemyRocket_Stand, 0.1f));
	animations.emplace(State::EnemyRocket_Sitting, Animation(SpriteId::EnemyRocket_Sitting, 0.1f));
	animations.emplace(State::EnemyRocket_Walking, Animation(SpriteId::EnemyRocket_Walking, 0.1f));
	animations.emplace(State::Explode, Animation(SpriteId::Explode, 0.25f));

}

void EnemyRocket::SetState(State state)
{
	const auto oldHeight = GetHeight();
	if (state!=State::EnemyGun_TakeDamage) VisibleObject::SetState(state); //this state dont have const animation
	pos.y += oldHeight - GetHeight();

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
	case State::EnemyGun_TakeDamage:
		OnFlasing(true);
		if (health <= 0) //die
		{
			vel.x = -nx * FALL_BACK;
		}
		
		break;
	case State::Explode:
		vel.x = 0;
		break;
	}
}

void EnemyRocket::SpawnRocket(float cycle)
{
	static Counter curCounter;
	if (curState == State::EnemyGun_TakeDamage) return;
	if (curCounter.CanExcuteCommand(cycle, false))
	{
		grid->SpawnObject(std::make_unique<BulletEnemyRocket>(nx, 1, pos));
	}
}

void EnemyRocket::TakeDamage(int damage)
{
	assert(damage > 0);
	health -= damage;
	SetState(State::EnemyGun_TakeDamage);
}

void EnemyRocket::Update(float dt, const std::vector<GameObject*>& coObjects)
{
	//regular update
	pos.x += vel.x*dt;
	pos.y += vel.y*dt;
	//take damamge
	if (isFlashing) {
		timeTakenDamaged += GameTimer::Dt();
		OnFlasing();
		//if take damage and health = 0, flasing in 0,5s then change to explode, keep 2 sprite in 0,2 s fit and destroy
		if (timeTakenDamaged > 0.7f)
		{
			if (health<=0) SetState(State::Explode);
			timeTakenDamaged = 0;
			OnFlasing(false);
		}
	}
	if (curState == State::Explode)
	{
		timeExploding += GameTimer::Dt();
		if (timeExploding>0.4f)
		{
			SetState(State::Destroyed);
			timeExploding = 0;
		}
	}

	// spawn bullet every 0.4s
	//SpawnRocket(4.0f);

	// update animations
	animations.at(curState).Update(dt);

}
