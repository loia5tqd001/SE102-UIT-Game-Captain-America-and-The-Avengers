#pragma once
#include "Counter.h"
#include "Enemy.h"

class EnemyGun : public Enemy
{
private:
	static constexpr float JUMP_SPEED         =  50.0f;
	static constexpr float FALL_BACK	      =   50.0f;

	float walkingSpeed = 15.0f;
	bool justShoot = false;

	void OnBehaviorShoot();
	void OnBehaviorRunOnly();
	void OnBehaviorAmbush();

public:
	EnemyGun(Behaviors behavior, const Data& behaviorData, Vector2 spawnPos, Grid* grid);

	void SetState(State state) override;
	void SpawnBullet();

	void Update(float dt, const std::vector<GameObject*>& coObjects) override;
};

