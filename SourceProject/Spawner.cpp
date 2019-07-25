#include "pch.h"
#include "Spawner.h"
#include "EnemyGun.h"
#include "EnemyRocket.h"

Spawner::Spawner(Vector2 pos, UINT w, UINT h, Behaviors behav, Vector2 objSpawnPos, int expectCapNx, Data&& data, Grid* grid) :
	InvisibleObject(pos, w, h),
	objectBehavior(behav),
	objSpawnPos(objSpawnPos),
	expectCapNx(expectCapNx),
	//behaviorData(data),
	grid(grid)
{
}

void Spawner::Update(float dt, const std::vector<GameObject*>& coObjects)
{	
	if (!enemy) 
	{
		if (readyToSpawn) // go spawn first!!
		{
			return;
		}

		// if spawner is out of camera
		auto cambox = Camera::Instance().GetBBox();
		if (!cambox.IsIntersect({ objSpawnPos, 24u, 32u })) // 24-32: normal size of enemy
		{
			readyToSpawn = true;
		}
	}
	else if (enemy->GetState() == State::Destroyed)
	{
		enemy.reset();
	}
}

void Spawner::OnCollideWithCap(Captain* cap)
{
	if (!readyToSpawn) return;
	if (cap->GetNx() != expectCapNx) return;
	else {
		switch (objectBehavior)
		{
			case Behaviors::EnemyGun_Shoot    : 
			case Behaviors::EnemyGun_ShootFast: 
			case Behaviors::EnemyGun_RunOnly  :
				enemy = std::make_shared<EnemyGun>(objectBehavior, objSpawnPos, cap, grid);
				grid->SpawnObject(enemy);
				readyToSpawn = false;
				break;

			case Behaviors::EnemyRocket_ShootStraight :
			case Behaviors::EnemyRocket_ShootCross    :
			case Behaviors::EnemyRocket_BackAndForth  :
				enemy = std::make_shared<EnemyRocket>(objectBehavior, objSpawnPos, cap, grid);
				grid->SpawnObject(enemy);
				readyToSpawn = false;
				if (objectBehavior == Behaviors::EnemyRocket_ShootStraight) enemy->is_debugging = true;
				break;

			default:
				ThrowMyException("Can't spawn enemy of behavior: ", (int)objectBehavior);
		}
	}
}

