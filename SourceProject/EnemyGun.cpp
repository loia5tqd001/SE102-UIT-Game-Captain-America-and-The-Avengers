#include "pch.h"
#include "EnemyGun.h"
#include "Captain.h"
#include "BulletEnemyGun.h"

EnemyGun::EnemyGun(Behaviors behavior, const Data& behaviorData, Vector2 spawnPos, Grid* grid) :
	Enemy(behavior, std::move(behaviorData), State::EnemyGun_BeforeExplode, 1, spawnPos, grid)
{
	animations.emplace(State::EnemyGun_BeforeExplode, Animation(SpriteId::EnemyGun_BeforeExplode, 0.3f));
	animations.emplace(State::EnemyGun_Walking, Animation(SpriteId::EnemyGun_Walking, 0.3f));
	animations.emplace(State::EnemyGun_Stand, Animation(SpriteId::EnemyGun_Stand, 0.8f));
	animations.emplace(State::EnemyGun_Sitting, Animation(SpriteId::EnemyGun_Sitting, 1.2f));

	switch (behavior)
	{
		case Behaviors::EnemyGun_ShootFast:
			animations.emplace(State::EnemyGun_Stand, Animation(SpriteId::EnemyGun_Stand, 0.1f));
			animations.emplace(State::EnemyGun_Sitting, Animation(SpriteId::EnemyGun_Sitting, 0.1f));
			SetState(State::EnemyGun_Sitting);
			break;
		case Behaviors::EnemyGun_Shoot:
			SetState(State::EnemyGun_Sitting);
			break;
		case Behaviors::EnemyGun_RunOnly:
			SetState(State::EnemyGun_Walking);
			walkingSpeed *= 3;
			// TODO: vel.x = toward captain * speed * 2
			break;
		case Behaviors::EnemyGun_Ambush:
			SetState(State::EnemyGun_Walking);
			break;
		default:
			ThrowMyException("Wrong behavior for enemy gun!!");
	}
}

void EnemyGun::OnBehaviorShoot()
{
	assert(behavior == Behaviors::EnemyGun_Shoot ||
	       behavior == Behaviors::EnemyGun_ShootFast);

	// Sit -> Stand -> Shoot -> Stand -> Repeat
	if (animations.at(curState).IsDoneCycle())
	switch (curState)
	{
		case State::EnemyGun_Sitting:
			SetState(State::EnemyGun_Stand);
			break;

		case State::EnemyGun_Stand:
			if (!justShoot) {
				SpawnBullet();
				justShoot = true;
			}
			else {
				SetState(State::EnemyGun_Sitting);
				justShoot = false;
			}

		case State::EnemyGun_BeforeExplode:
		case State::Explode:
		case State::Destroyed:
			break;
		default:
			ThrowMyException("Wrong state for EnemyGun_Shoot");
	}
}

void EnemyGun::OnBehaviorRunOnly()
{
	assert(behavior == Behaviors::EnemyGun_RunOnly);
	assert(curState == State::EnemyGun_Walking ||
	       curState == State::EnemyGun_BeforeExplode ||
	       curState == State::Explode ||
	       curState == State::Destroyed);
}

void EnemyGun::OnBehaviorAmbush()
{
	assert(behavior == Behaviors::EnemyGun_Ambush);

	// Walking Right -> Stand&Shoot -> Walking Right-> Repeat 
	if (animations.at(curState).IsDoneCycle())
	switch (curState)
	{
		case State::EnemyGun_Walking:
			SetState(State::EnemyGun_Stand);
			break;

		case State::EnemyGun_Stand:
			if (!justShoot) {
				SpawnBullet();
				justShoot = true;
			}
			else {
				SetState(State::EnemyGun_Walking);
				justShoot = false;
			}

		case State::EnemyGun_BeforeExplode:
		case State::Explode:
		case State::Destroyed:
			break;
		default:
			ThrowMyException("Wrong state for behavior EnemyGun_Ambush");
	}
}

void EnemyGun::Update(float dt, const std::vector<GameObject*>& coObjects)
{
	switch (behavior)
	{
		case Behaviors::EnemyGun_Shoot:
		case Behaviors::EnemyGun_ShootFast:
			OnBehaviorShoot();
			break;
		case Behaviors::EnemyGun_RunOnly:
			OnBehaviorRunOnly();
			break;
		case Behaviors::EnemyGun_Ambush:
			OnBehaviorAmbush();
			break;
		default:
			ThrowMyException("Wrong behavior for enemy rocket!!");
	}

	pos.x += vel.x*dt;
	pos.y += vel.y*dt;

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
		vel.x = nx * walkingSpeed;
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

void EnemyGun::SpawnBullet() 
{
	if (isFlashing) return;
	if (curState == State::EnemyGun_Sitting)
	{
		const auto bulletPos = pos + Vector2{ 24.0f, 10.0f };
		grid->SpawnObject(std::make_unique<BulletEnemyGun>(nx, this, bulletPos));
		Sounds::PlayAt(SoundId::BulletNormal);
	}
	else if (curState == State::EnemyGun_Stand)
	{
		const auto bulletPos = pos + Vector2{ 24.0f, 3.0f };
		grid->SpawnObject(std::make_unique<BulletEnemyGun>(nx, this, bulletPos));
		Sounds::PlayAt(SoundId::Explosion);
	}
}
