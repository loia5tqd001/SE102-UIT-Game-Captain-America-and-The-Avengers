#include "pch.h"
#include "DynamiteNapalm.h"


DynamiteNapalm::DynamiteNapalm(Behaviors behavior, Data && behaviorData, Vector2 spawnPos, Vector2 vel, int nx, Grid * grid, Captain & refcap):
	Enemy(behavior,behaviorData,State::DynamiteNapalm_BeforeExplode,DYNAMITENAPALM_HEALTH,pos,grid),
	cap(refcap)
{

}

void DynamiteNapalm::SetState(State state)
{
}

void DynamiteNapalm::SpawnDynamite()
{
}

void DynamiteNapalm::Update(float dt, const std::vector<GameObject*>& coObjects)
{
}

void DynamiteNapalm::SpawnBullet()
{
}

void DynamiteNapalm::HandleCollisions(float dt, const std::vector<GameObject*>& coObjects)
{
}
