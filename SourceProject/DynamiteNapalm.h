#pragma once
#include "Enemy.h"

class DynamiteNapalm : public Enemy
{
private:
	static constexpr auto FALLING_SPEED = 250.0f;
	static constexpr auto RUNNING_SPEED = 80.0f;
	static constexpr auto RUNNING_BEFORE_DEAD_SPEED = 1.2f*RUNNING_SPEED;
	static constexpr auto NUM_FIRE_BULLET = 2;
	static constexpr auto DEFAULT_HEALTH = 15;
	static constexpr auto maxHealthHeadless = 5;
	static constexpr auto TIME_TO_HEADLESS_SHOOT = 1.5f;
	static constexpr auto FAR_DISTANCE = 70.0f;


	bool dynamiteThrown = false;
	Captain& cap;
	float posXDesination;
	bool onTheGround = false;

	float holdingDistance = 0.0f;
	float maxDistance_IntactRunning = 50.0f;
	int numFireBulletFired = 0;
	float holdTime = TIME_TO_HEADLESS_SHOOT; //Shoot immediately
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

