#pragma once
#include "Enemy.h"

class EnemyFly : public Enemy
{
	static constexpr float MOVING_SPEED = 70.0f;
	static constexpr float FALLING_SPEED = 100.0f;
	static constexpr float FALLING_TIME = 20.0f;
	static constexpr float HOR_DISTANCE = 150.0f;
	static constexpr float VER_DISTANCE = 50.0f;

	Vector2 spawnPos;
	Captain *cap;
	float fallingCounter = 0;
	float counterSpawnBullet = 0;

	float countHorDistance = 0;
	float countVerDistance = 0;
public:
	EnemyFly(Vector2 spawnPos, Grid* grid, Captain *cap);
	void Update(float dt, const std::vector<GameObject*>& coObjects) override;
	void SpawnBullet(); //some enemy dont have bullets, pls dont pull this on father class
	void SetState(State state) override;
	void TakeDamage(int damage) override;
};

