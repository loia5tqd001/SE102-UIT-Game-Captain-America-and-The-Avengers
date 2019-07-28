#pragma once
#include "Enemy.h"
class DynamiteNapalm : public Enemy
{
private:
	static constexpr float NORMAL_RUNNING_SPEED = 80.0f;
	static constexpr float HEADLESS_RUNNING_SPEED = 120.0f;
	static constexpr int DYNAMITENAPALM_HEALTH = 100;

	Captain* cap;
public:
	DynamiteNapalm(Vector2 spawnPos, int nx, Grid * grid, Captain* ptrcap);
	void SetState(State state) override;
	void SpawnDynamite();
	void Update(float dt, const std::vector<GameObject*>& coObjects) override;
	void SpawnBullet();
	void TakeDamage(int damage);
	void HandleCollisions(float dt, const std::vector<GameObject*>& coObjects);
};

