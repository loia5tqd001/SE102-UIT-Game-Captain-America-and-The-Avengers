#include "pch.h"
#include "AmbushTrigger.h"
#include "EnemyGun.h"
#include "EnemyRocket.h"
#include "Spawner.h"

static auto& sceneManger = SceneManager::Instance();
AmbushTrigger* AmbushTrigger::instance = nullptr;

AmbushTrigger::AmbushTrigger(Vector2 pos, UINT w, UINT h, const RectF& lockRegion, Grid* grid) :
	InvisibleObject(pos, w, h),
	lockCamera(lockRegion),
	lockCaptain(lockRegion.GetTrim(5, 0, 20, 0)),
	enemyGunSpawnPos(lockRegion.GetTopLeft() + Vector2{ -22.0f, 165.0f }),
	enemyRocketSpawnPos(lockRegion.GetTopLeft() + Vector2{ 222.0f, 157.0f }),
	grid(grid)
{
}

const RectF& AmbushTrigger::GetLockCamera() const
{
	assert(state == State::Ambush_Being);
	return lockCamera;
}

const RectF& AmbushTrigger::GetLockCaptain() const
{
	assert(state == State::Ambush_Being);
	return lockCaptain;
}

void AmbushTrigger::OnCollideWithCap(Captain* cap)
{
	if (state == State::Ambush_Being || state == State::Ambush_HasDone) return;

	state = State::Ambush_Being;
	captain = cap;

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
	SpawnEnemyGun();
	SpawnEnemyRocket();

	Sounds::StopAt(sceneManger.GetCurScene().GetBgMusic());
	Sounds::PlayLoop(SoundId::Ambush);
}

void AmbushTrigger::SpawnEnemyGun()
{
	enemyGun = std::make_shared<EnemyGun>(Behaviors::EnemyGun_Ambush, enemyGunSpawnPos, captain, grid);
	grid->SpawnObject(enemyGun);
}

void AmbushTrigger::SpawnEnemyRocket()
{
	enemyRocket = std::make_shared<EnemyRocket>(Behaviors::EnemyRocket_Ambush, enemyRocketSpawnPos, captain, grid);
	grid->SpawnObject(enemyRocket);
}

void AmbushTrigger::Update(float dt, const std::vector<GameObject*>& coObjects)
{
	if (state == State::Ambush_NotYet || state == State::Ambush_HasDone) return;

	if (countEnemyGun <= 0 && countEnemyRocket <= 0 &&
		enemyGun->GetState() == State::Destroyed &&
		enemyRocket->GetState() == State::Destroyed) // done ambush
	{
		state = State::Ambush_HasDone;
		for (auto spawner : spawners) spawner->SetActive(true);
		Sounds::StopAt(SoundId::Ambush);
		Sounds::PlayLoop(sceneManger.GetCurScene().GetBgMusic());
		enemyGun.reset();
		enemyRocket.reset();
		return;
	}

	if (enemyGun->GetState() == State::Destroyed) {
		countEnemyGun--;
		if (countEnemyGun > 0 || countEnemyRocket > 0) SpawnEnemyGun();
	}
	else if (!enemyGun->VisibleObject::GetBBox().IsIntersect(lockCamera)) // going too far
	{ 
		enemyGun->SetState(State::Destroyed);
		countEnemyGun++;
	}

	if (enemyRocket->GetState() == State::Destroyed)
	{
		countEnemyRocket--;
		if (countEnemyGun > 0 || countEnemyRocket > 0) SpawnEnemyRocket();
	}
	else if (!enemyRocket->VisibleObject::GetBBox().IsIntersect(lockCamera)) // going too far
	{
		enemyRocket->SetState(State::Destroyed);
		countEnemyRocket++;
	}
}

void AmbushTrigger::SetInstance(AmbushTrigger* inst)
{
	instance = inst;
}

AmbushTrigger*& AmbushTrigger::Instance()
{
	return instance;
}
