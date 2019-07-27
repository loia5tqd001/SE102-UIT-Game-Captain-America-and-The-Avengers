#pragma once
#include "Enemy.h"
class DynamiteNapalm : public Enemy
{
private:
	static constexpr float NORMAL_RUNNING_SPEED = 80.0f;
	static constexpr float HEADLESS_RUNNING_SPEED = 120.0f;
	static constexpr int DYNAMITENAPALM_HEALTH = 100;

	Captain& cap;
public:
	DynamiteNapalm(Behaviors behavior, Data&& behaviorData, Vector2 spawnPos, Vector2 vel, int nx, Grid * grid, Captain& refcap);
	void SetState(State state) override;
	void SpawnDynamite();
	void Update(float dt, const std::vector<GameObject*>& coObjects) override;
	void SpawnBullet();
	void HandleCollisions(float dt, const std::vector<GameObject*>& coObjects);
};

