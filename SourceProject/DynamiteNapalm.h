#pragma once
#include "Enemy.h"

class DynamiteNapalm : public Enemy
{
private:
	static constexpr float FALLING_SPEED = 100.0f;
	static constexpr float RUNNING_SPEED = 80.0f;
	static constexpr float RUNNING_BEFORE_DEAD_SPEED = 1.5f*RUNNING_SPEED;

	bool dynamiteThrown = false;
	Captain& cap;
	float posXDesination;
	bool OnBehavior(Behaviors behavior);
	bool onTheGround = false;
public:
	DynamiteNapalm(Behaviors behavior, Data&& behaviorData, Vector2 spawnPos, Vector2 vel, int nx, Grid * grid, Captain& cap);
	void SetState(State state) override;
	void Update(float dt, const std::vector<GameObject*>& coObjects) override;
	void HandleCollisions(float dt, const std::vector<GameObject*>& coObjects);
	void SpawnDynamite();
	void SpawnFireBullet();

};

