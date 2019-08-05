#include "pch.h"
#include "Bunker.h"
#include "BulletBunker.h"

Bunker::Bunker(State initState, Vector2 spawnPos, Grid* grid) :
	Enemy( {}, {}, initState, -1, spawnPos, grid)
{
	animations.emplace(State::Bunker_Idle_0, Animation(SpriteId::Bunker_Idle_0, 1.0f));
	animations.emplace(State::Bunker_Idle_1, Animation(SpriteId::Bunker_Idle_1, 1.0f));
	animations.emplace(State::Bunker_Idle_2, Animation(SpriteId::Bunker_Idle_2, 1.0f));
	animations.emplace(State::Bunker_Idle_3, Animation(SpriteId::Bunker_Idle_3, 1.0f));
	animations.emplace(State::Bunker_Idle_4, Animation(SpriteId::Bunker_Idle_4, 1.0f));
	animations.emplace(State::Bunker_Idle_5, Animation(SpriteId::Bunker_Idle_5, 1.0f));
	animations.emplace(State::Bunker_Idle_6, Animation(SpriteId::Bunker_Idle_6, 1.0f));
	animations.emplace(State::Bunker_Idle_7, Animation(SpriteId::Bunker_Idle_7, 1.0f));
	animations.emplace(State::Bunker_Spin  , Animation(SpriteId::Bunker_Spin  , 0.02f));
	lastIdleState = (int)initState - (int)State::Bunker_Idle_0;
}

void Bunker::SpawnBullet()
{
	static constexpr auto BULLET_SPEED = 150.0f;
	static const auto SQRT_BULLET_SPEED = std::sqrtf(2.0f) * 75.0f;

	auto posSpawn = pos + Vector2{ 5.0f, 5.0f }; // always spawn bullet at the center of bunker
	Vector2 velSpawn;
	switch (curState)
	{
		case State::Bunker_Idle_0: velSpawn = { 0.0f               , -BULLET_SPEED      }; break;
		case State::Bunker_Idle_1: velSpawn = { -SQRT_BULLET_SPEED , -SQRT_BULLET_SPEED }; break;
		case State::Bunker_Idle_2: velSpawn = { -BULLET_SPEED      , 0.0f               }; break;
		case State::Bunker_Idle_3: velSpawn = { -SQRT_BULLET_SPEED , SQRT_BULLET_SPEED  }; break;
		case State::Bunker_Idle_4: velSpawn = { 0.0f               , BULLET_SPEED       }; break;
		case State::Bunker_Idle_5: velSpawn = { SQRT_BULLET_SPEED  , SQRT_BULLET_SPEED  }; break;
		case State::Bunker_Idle_6: velSpawn = { BULLET_SPEED       , 0.0f               }; break;
		case State::Bunker_Idle_7: velSpawn = { SQRT_BULLET_SPEED  , -SQRT_BULLET_SPEED }; break;
		default: AssertUnreachable();
	}
	grid->SpawnObject(std::make_unique<BulletBunker>(posSpawn, velSpawn));
}

void Bunker::TakeDamage(int damage)
{
	if (AmbushTrigger::Instance()->GetState() == State::Ambush_Being) return;
	SetState(State::Bunker_Spin);
}

void Bunker::Update(float dt, const std::vector<GameObject*>& coObjects)
{
	// Bunkers should be respawned if ambush is done. But who cares.
	//if (AmbushTrigger::Instance()->GetState() != State::Ambush_Being)
	//{
	//	if (curState == State::Destroyed)
	//	{
	//		if (std::abs(pos.x - Camera::Instance().GetPos().x) > 100.0f)
	//			SetState(State((int)State::Bunker_Idle_0 + lastIdleState));
	//	}
	//}

	animations.at(curState).Update(dt);
	if (curState == State::Explode) // call by ambush trigger
	{
		if (animations.at(curState).IsDoneCycle())
		{
			SetState(State::Destroyed);
		}
	}
	else if (curState == State::Bunker_Spin)
	{
		if (animations.at(curState).IsDoneCycle() && --countSpin <= 0)
		{
			lastIdleState = ++lastIdleState % 8;
			auto nextState = (int)State::Bunker_Idle_0 + lastIdleState;
			SetState( (State) nextState );
			countSpin = 7;
		}
	}
	else // idle
	{
		if (animations.at(curState).IsDoneCycle())
		{
			SpawnBullet();
		}
	}
}

RectF Bunker::GetBBox() const
{
	if (curState == State::Destroyed || curState == State::Explode) return {};
	else return Enemy::GetBBox();
}
