#pragma once
#include "Counter.h"
#include "Enemy.h"

class EnemyRocket : public Enemy
{
private:
	static constexpr float WALKING_SPEED = 55.0f;

	void OnBehaviorShoot();
	void BackAndForthJump();
	void OnBehaviorBackAndForth();
	void OnBehaviorAmbush();

	int rocketType = 0;
	int countWalkStep = 0;
	bool justShoot = false;
	bool isLastForth = false; 
	bool isInJumpBackNForth = true;
	State lastState = State::EnemyRocket_Walking;
	Captain* cap;

public:
	EnemyRocket(Behaviors behavior, Vector2 spawnPos, Captain* cap, Grid* grid);

	void SetState(State state) override;
	void SpawnRocket();

	void Update(float dt, const std::vector<GameObject*>& coObjects) override;
};

