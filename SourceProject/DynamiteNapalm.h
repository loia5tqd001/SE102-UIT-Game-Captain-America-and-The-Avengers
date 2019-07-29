#pragma once
#include "Enemy.h"

class DynamiteNapalm : public Enemy
{
private:
	static constexpr float FALLING_SPEED = 250.0f;
	static constexpr float RUNNING_SPEED = 80.0f;
	static constexpr float RUNNING_BEFORE_DEAD_SPEED = 1.5f*RUNNING_SPEED;
	static constexpr int NUM_FIRE_BULLET = 2;
	static constexpr int DEFAULT_HEALTH = 15;
	static constexpr int maxHealthHeadless = 5;

	bool dynamiteThrown = false;
	Captain& cap;
	float posXDesination;
	bool onTheGround = false;

	float holdingDistance = 0.0f;
	float maxDistance_IntactRunning = 120.0f;
	int numFireBulletFired = 0;
	bool OnBehavior(Behaviors behavior, float dt);
public:
	DynamiteNapalm(Behaviors behavior, Data&& behaviorData, Vector2 spawnPos, Vector2 vel, int nx, Grid * grid, Captain& cap);
	void SetState(State state) override;
	void Update(float dt, const std::vector<GameObject*>& coObjects) override;
	void HandleCollisions(float dt, const std::vector<GameObject*>& coObjects);
	void SpawnDynamite();
	void SpawnFireBullet();
	bool CanTakeDamage();
};

