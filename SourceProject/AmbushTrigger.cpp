#include "pch.h"
#include "AmbushTrigger.h"
#include "EnemyGun.h"
#include "EnemyRocket.h"

AmbushTrigger::AmbushTrigger(Vector2 pos, UINT w, UINT h, const RectF& lockRegion, Grid* grid) :
	InvisibleObject(pos, w, h),
	lockRegion(lockRegion),
	grid(grid)
{
	const Vector2 topleftLock = lockRegion.GetTopLeft();
	enemyGunSpawnPos = topleftLock + Vector2{ -22.0f, 165.0f };
	enemyRocketSpawnPos = topleftLock + Vector2{ 222.0f, 157.0f };
}

State AmbushTrigger::GetState() const
{
	if (isDestroyed) return State::Destroyed;
	else return InvisibleObject::GetState();
}

void AmbushTrigger::Update(float dt, const std::vector<GameObject*>& coObjects)
{
	if (!isActive) return;

	static Camera& cam = Camera::Instance();
	static EnemyGun* enemyGun = nullptr;
	static EnemyRocket* enemyRocket = nullptr;
	int countEnemyGun = 3, countEnemyRocket = 3;

	if (!enemyGun->GetBBox().IsIntersect(cam.GetBBox())) // respawn enemy when out of camera
	{
		enemyGun->SetState(State::Destroyed);
		enemyGun = dynamic_cast<EnemyGun*>( grid->SpawnObject(
			std::make_unique<EnemyGun>(Behaviors::EnemyGun_Ambush, Data{}, enemyGunSpawnPos, grid)));
	}
	else if (enemyGun->GetState() == State::Destroyed) // being killed by Captain
	{
		enemyGun = dynamic_cast<EnemyGun*>( grid->SpawnObject(
			std::make_unique<EnemyGun>(Behaviors::EnemyGun_Ambush, Data{}, enemyGunSpawnPos, grid)));
		countEnemyGun--;
	}

	if (!enemyRocket->GetBBox().IsIntersect(cam.GetBBox()))
	{
		enemyRocket->SetState(State::Destroyed);
		enemyRocket = dynamic_cast<EnemyRocket*>( grid->SpawnObject(
			std::make_unique<EnemyRocket>(Behaviors::EnemyGun_Ambush, Data{}, enemyRocketSpawnPos, grid)));
	}
	else if (enemyRocket->GetState() == State::Destroyed) // being killed
	{
		enemyRocket = dynamic_cast<EnemyRocket*>( grid->SpawnObject(
			std::make_unique<EnemyRocket>(Behaviors::EnemyGun_Ambush, Data{}, enemyRocketSpawnPos, grid)));
		countEnemyRocket--;
	}

	if (countEnemyGun <= 0 && countEnemyRocket <= 0) // done ambush
	{
		countEnemyGun = countEnemyRocket = 3;
		enemyGun = nullptr, enemyRocket = nullptr;
		isDestroyed = true;
	}
}