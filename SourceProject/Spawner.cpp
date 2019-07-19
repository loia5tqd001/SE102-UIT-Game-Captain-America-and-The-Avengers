#include "pch.h"
#include "Spawner.h"
#include "EnemyGun.h"
#include "EnemyRocket.h"

Spawner::Spawner(Vector2 pos, UINT w, UINT h, Behaviors behav, Vector2 objSpawnPos, Data&& data, Grid* grid) :
	InvisibleObject(pos, w, h),
	objectBehavior(behav),
	objSpawnPos(objSpawnPos),
	behaviorData(data),
	grid(grid)
{
}

void Spawner::Update(float dt, const std::vector<GameObject*>& coObjects)
{
	// update if object it's holding is destroyed
	if (enemy == nullptr) return;
	else if (enemy->GetState() == State::Destroyed)
	{
		enemy = nullptr;
	}
}

void Spawner::OnCollideWithCap()
{
	if (enemy != nullptr) return; // if object's still alive
	else {
		switch (objectBehavior)
		{
			case Behaviors::EnemyGun_Shoot   : 
			case Behaviors::EnemyGun_RunOnly :
				enemy = dynamic_cast<EnemyGun*>( grid->SpawnObject(
					std::make_unique<EnemyGun>(objectBehavior, behaviorData, objSpawnPos, grid)));
				break;

			case Behaviors::EnemyRocket_ShootStraight :
			case Behaviors::EnemyRocket_ShootCross    :
			case Behaviors::EnemyRocket_BackAndForth  :
			case Behaviors::EnemyRocket_BackAndForth2 :
				enemy = dynamic_cast<EnemyRocket*>( grid->SpawnObject(
					std::make_unique<EnemyRocket>(objectBehavior, behaviorData, objSpawnPos, grid)));
				break;

			default:
				ThrowMyException("Can't spawn enemy of behavior: ", (int)objectBehavior);
		}
	}
}

