#include "pch.h"
#include "EnemyRocket.h"
#include "Captain.h"
#include "BulletEnemyRocket.h"


EnemyRocket::EnemyRocket(const Vector2 & spawnPos, const Vector2 & vel, int nx, Grid * grid) :
	Enemy(State::EnemyRocket_Stand, 3, spawnPos, vel, nx, grid)
{
	animations.emplace(State::EnemyRocket_Stand, Animation(SpriteId::EnemyRocket_Stand, 0.1f));
	animations.emplace(State::EnemyRocket_Sitting, Animation(SpriteId::EnemyRocket_Sitting, 0.1f));
	animations.emplace(State::EnemyRocket_TakeDamage, Animation(SpriteId::EnemyRocket_TakeDamage, 0.1f));
	animations.emplace(State::EnemyRocket_Walking, Animation(SpriteId::EnemyRocket_Walking, 0.1f));
	animations.emplace(State::Explode, Animation(SpriteId::Explode, 0.1f));
}

void EnemyRocket::SetState(State state)
{
	const auto oldHeight = GetHeight();
	VisibleObject::SetState(state);
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
		if (health <= 0) //die
		{
			vel.x = -nx * FALL_BACK;
		}
		else
		{
			vel.x = 0.0f;
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
		grid->SpawnObject(std::make_unique<BulletEnemyRocket>( pos, Vector2{ 100.0f, 0.0f }, nx));
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
	//counter
	if (curState == State::EnemyGun_TakeDamage) {
		static Counter takeDamageCounter;
		Enemy::OnFlasing();
		if (health > 0) {
			animations.at(State::EnemyGun_TakeDamage).Update(dt);
		}
		//if take damage and health = 0, flasing in 0,5s then change to explode, keep 2 sprite in 0,2 s fit and destroy
		else {
			if (takeDamageCounter.CanExcuteCommand(0.5f, true))
			{
				SetState(State::Explode);
			}
		}
	}
	if (curState == State::Explode)
	{
		static Counter ExplodeCounter;
		if (ExplodeCounter.CanExcuteCommand(0.2f, true))
		{
			SetState(State::Destroyed);
		}
	}

	// spawn bullet every 0.4s
	//SpawnRocket(4.0f);

	// update animations
	animations.at(curState).Update(dt);

}
