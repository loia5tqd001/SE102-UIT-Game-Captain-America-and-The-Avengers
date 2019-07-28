#include "pch.h"
#include "DynamiteNapalm.h"


DynamiteNapalm::DynamiteNapalm(Vector2 spawnPos, int nx, Grid * grid, Captain *ptrcap) :
	Enemy(behavior, behaviorData, State::DynamiteNapalm_Standing, DYNAMITENAPALM_HEALTH, pos, grid),
	cap(ptrcap)
{
	animations.emplace(State::DynamiteNapalm_FallFromTheSky, Animation(SpriteId::DynamiteNapalm_FallFromTheSky));
	animations.emplace(State::DynamiteNapalm_Standing, Animation(SpriteId::DynamiteNapalm_Standing, 1.0f));
	animations.emplace(State::DynamiteNapalm_Intact_Running, Animation(SpriteId::DynamiteNapalm_Intact_Running, 0.2f));
	animations.emplace(State::DynamiteNapalm_ThrowDynamite, Animation(SpriteId::DynamiteNapalm_ThrowDynamite, 0.5f));
	animations.emplace(State::DynamiteNapalm_Intact_Shooting, Animation(SpriteId::DynamiteNapalm_Intact_Shooting, 0.1f));
	animations.emplace(State::DynamiteNapalm_Intact_Injure, Animation(SpriteId::DynamiteNapalm_Intact_Injure, 0.1f));
	animations.emplace(State::DynamiteNapalm_Headless_Standing, Animation(SpriteId::DynamiteNapalm_Headless_Standing));
	animations.emplace(State::DynamiteNapalm_Headless_Running_Shooting, Animation(SpriteId::DynamiteNapalm_Headless_Running_Shooting));
	animations.emplace(State::DynamiteNapalm_BeforeExplode, Animation(SpriteId::DynamiteNapalm_Headless_Standing));

	beforeExplode = State::DynamiteNapalm_BeforeExplode;
}

void DynamiteNapalm::SetState(State state)
{
}

void DynamiteNapalm::SpawnDynamite()
{
}

void DynamiteNapalm::Update(float dt, const std::vector<GameObject*>& coObjects)
{
	UpdateAnimation(dt);
}

void DynamiteNapalm::SpawnBullet()
{

}

void DynamiteNapalm::TakeDamage(int damage)
{
	switch (curState)
	{
	case State::DynamiteNapalm_Headless_Running_Shooting:
	case State::DynamiteNapalm_Headless_Standing:
		Enemy::TakeDamage(damage);
	default:
		break;
	}
}

void DynamiteNapalm::HandleCollisions(float dt, const std::vector<GameObject*>& coObjects)
{
}
