#include "pch.h"
#include "AmbushTrigger.h"
#include "EnemyGun.h"
#include "EnemyRocket.h"
#include "Spawner.h"

static auto& sceneManger = SceneManager::Instance();

AmbushTrigger::AmbushTrigger(Vector2 pos, UINT w, UINT h, const RectF& lockRegion, Grid* grid) :
	InvisibleObject(pos, w, h),
	lockRegion(lockRegion),
	grid(grid)
{
	const Vector2 topleftLock = lockRegion.GetTopLeft();
	enemyGunSpawnPos = topleftLock + Vector2{ -22.0f, 165.0f };
	enemyRocketSpawnPos = topleftLock + Vector2{ 222.0f, 157.0f };
}

void AmbushTrigger::Active()
{
	for (auto& o : grid->GetObjectsInViewPort()) 
	{
		if (auto enemy = dynamic_cast<Enemy*>(o)) {
			enemy->SetState(State::Explode);
		}
		else if (auto spawner = dynamic_cast<Spawner*>(o)) {
			spawner->SetActive(false);
			spawners.emplace_back(spawner);
		}
	}

	isActive = true;
	Camera::Instance().SetLockRegion( lockRegion );
	Sounds::StopAt(sceneManger.GetCurScene().GetBgMusic());
	Sounds::PlayLoop(SoundId::Ambush);
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

	if (countEnemyGun <= 0 && countEnemyRocket <= 0) // done ambush
	{
		countEnemyGun = countEnemyRocket = 3;
		enemyGun = nullptr, enemyRocket = nullptr;
		isDestroyed = true;
		for (auto spawner : spawners) spawner->SetActive(true);
		Sounds::StopAt(SoundId::Ambush);
		Sounds::PlayLoop(sceneManger.GetCurScene().GetBgMusic());
		Camera::Instance().SetLockRegion( {} );
		return;
	}

	if (enemyGun == nullptr)
	{
		enemyGun = dynamic_cast<EnemyGun*>( grid->SpawnObject(
			std::make_unique<EnemyGun>(Behaviors::EnemyGun_Ambush, Data{}, enemyGunSpawnPos, grid)));
	}
	else if (!enemyGun->GetBBox().IsNone()) // not before explode or explode
	{
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
	}


	if (enemyRocket == nullptr)
	{
		enemyRocket = dynamic_cast<EnemyRocket*>( grid->SpawnObject(
			std::make_unique<EnemyRocket>(Behaviors::EnemyRocket_Ambush, Data{}, enemyRocketSpawnPos, grid)));
	}
	else if (!enemyRocket->GetBBox().IsNone())
	{
		if (!enemyRocket->GetBBox().IsIntersect(cam.GetBBox()))
		{
			enemyRocket->SetState(State::Destroyed);
			enemyRocket = dynamic_cast<EnemyRocket*>( grid->SpawnObject(
				std::make_unique<EnemyRocket>(Behaviors::EnemyRocket_Ambush, Data{}, enemyRocketSpawnPos, grid)));
		}
		else if (enemyRocket->GetState() == State::Destroyed) // being killed
		{
			enemyRocket = dynamic_cast<EnemyRocket*>( grid->SpawnObject(
				std::make_unique<EnemyRocket>(Behaviors::EnemyRocket_Ambush, Data{}, enemyRocketSpawnPos, grid)));
			countEnemyRocket--;
		}
	}
}