#pragma once
#include "Counter.h"
#include "Enemy.h"

class EnemyGun : public Enemy
{
private:
	static constexpr float JUMP_SPEED    = 50.0f;

	float WALKING_SPEED = 55.0f;
	bool justShoot = false;
	Captain* cap;

	void OnBehaviorShoot();
	void OnBehaviorRunOnly();
	void OnBehaviorAmbush();

public:
	EnemyGun(Behaviors behavior, Vector2 spawnPos, Captain* cap, Grid* grid);

	void SetState(State state) override;
	void SpawnBullet();

	void Update(float dt, const std::vector<GameObject*>& coObjects) override;
};

