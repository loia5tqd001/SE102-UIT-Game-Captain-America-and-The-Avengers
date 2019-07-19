#include "pch.h"
#include "EnemyRocket.h"
#include "Captain.h"
#include "BulletEnemyRocket.h"

EnemyRocket::EnemyRocket(Behaviors behavior, const Data& behaviorData, Vector2 spawnPos, Grid* grid) :
	Enemy(behavior, std::move(behaviorData), State::EnemyRocket_BeforeExplode, 2, spawnPos, grid)
{
	animations.emplace(State::EnemyRocket_BeforeExplode, Animation(SpriteId::EnemyRocket_BeforeExplode, 0.2f));
	animations.emplace(State::EnemyRocket_Walking, Animation(SpriteId::EnemyRocket_Walking, 0.3f));
	animations.emplace(State::EnemyRocket_Stand, Animation(SpriteId::EnemyRocket_Stand, 0.1f));
	animations.emplace(State::EnemyRocket_Sitting, Animation(SpriteId::EnemyRocket_Sitting, 0.2f));

	SetState(State::EnemyRocket_Walking);
	switch (behavior)
	{
		case Behaviors::EnemyRocket_ShootStraight:
			break;
		case Behaviors::EnemyRocket_ShootCross:
			rocketType = 1;
			break;
		case Behaviors::EnemyRocket_BackAndForth:
			break;
		case Behaviors::EnemyRocket_Ambush:
			break;
		default:
			ThrowMyException("Wrong behavior for enemy rocket!!");
	}
}

void EnemyRocket::OnBehaviorShoot()
{
	assert(behavior == Behaviors::EnemyRocket_ShootStraight ||
	       behavior == Behaviors::EnemyRocket_ShootCross);

	// Walk -> Stand&Shoot -> Sit&Shoot-> Stand&Shoot -> Repeat
	if (animations.at(curState).IsDoneCycle())
	switch (curState)
	{
		case State::EnemyRocket_Walking:
			if (++countWalkStep >= 4) 
				SetState(State::EnemyRocket_Sitting);
			break;

		case State::EnemyRocket_Stand:
			if (!justShoot) {
				SpawnRocket();
				justShoot = true;
			}
			else {
				SetState(State::EnemyRocket_Sitting);
				justShoot = false;
			}

		case State::EnemyRocket_Sitting:
			if (!justShoot) {
				SpawnRocket();
				justShoot = true;
			}
			else {
				SetState(State::EnemyRocket_Stand);
				justShoot = false;
			}

		case State::EnemyRocket_BeforeExplode:
		case State::Explode:
		case State::Destroyed:
			break;
		default:
			ThrowMyException("Wrong state for behavior EnemyRocket_Shoot");
	}
}

void EnemyRocket::OnBehaviorBackAndForth()
{
	assert(behavior == Behaviors::EnemyRocket_BackAndForth);
	assert(rocketType == 0); // straight rocket only

	// if y == tren buc && nx == -1 -> nhay xuong
	// if y == tren buc && nc == 1 -> nhay len

	if (animations.at(curState).IsDoneCycle())
	switch (curState)
	{
		case State::EnemyRocket_Walking:
			if (++countWalkStep >= 4) {
				SetState(State::EnemyRocket_Stand);
				countWalkStep = 0;
				isLastForth = !isLastForth;
			}
			break;

		case State::EnemyRocket_Stand:
			SetState(State::EnemyRocket_Sitting);
			break;

		case State::EnemyRocket_Sitting:
			SpawnRocket();
			SetState(State::EnemyRocket_Walking);
			break;

		case State::EnemyRocket_BeforeExplode:
		case State::Explode:
		case State::Destroyed:
			break;
		default:
			ThrowMyException("Wrong state for behavior EnemyRocket_BackAndForth");
	}
}

void EnemyRocket::OnBehaviorAmbush()
{
	// Walking left only
	assert(behavior == Behaviors::EnemyRocket_Ambush);
	assert(curState == State::EnemyRocket_Walking ||
		   curState == State::EnemyRocket_BeforeExplode ||
		   curState == State::Explode ||
		   curState == State::Destroyed);
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
		vel.x = (isLastForth ? -1 : 1) * nx * WALKING_SPEED;
		break;
	case State::EnemyRocket_Sitting:
		vel.x = 0.0f;
		break;
	case State::EnemyRocket_BeforeExplode:
		vel.x = -nx * FALL_BACK;
		break;
	case State::Explode:
		vel.x = 0;
		break;
	}
}

void EnemyRocket::SpawnRocket()
{
	if (isFlashing) return;
	grid->SpawnObject(std::make_unique<BulletEnemyRocket>(nx, rocketType, pos));
}

void EnemyRocket::Update(float dt, const std::vector<GameObject*>& coObjects)
{
	switch (behavior)
	{
		case Behaviors::EnemyRocket_ShootStraight:
		case Behaviors::EnemyRocket_ShootCross:
			OnBehaviorShoot();
			break;
		case Behaviors::EnemyRocket_BackAndForth:
			OnBehaviorBackAndForth();
			break;
		case Behaviors::EnemyRocket_Ambush:
			OnBehaviorAmbush();
			break;
		default:
			ThrowMyException("Wrong behavior for enemy rocket!!");
	}

	pos.x += vel.x*dt;
	pos.y += vel.y*dt;

	UpdateAnimation(dt);
}
