#pragma once
#include "Counter.h"
#include "Enemy.h"

class EnemyRocket : public Enemy
{
private:
	static constexpr float WALKING_SPEED = 15.0f;
	static constexpr float JUMP_SPEED = 50.0f;
	static constexpr float GRAVITY = 50.0f;
	static constexpr float FALL_BACK = 50.0f;

public:
	EnemyRocket(Behaviors behavior, const Data& behaviorData, Vector2 spawnPos, Grid* grid);

	void SetState(State state) override;
	void SpawnRocket(float cycle);

	void Update(float dt, const std::vector<GameObject*>& coObjects) override;
};

