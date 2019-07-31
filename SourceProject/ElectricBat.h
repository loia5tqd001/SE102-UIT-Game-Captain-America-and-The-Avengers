#pragma once
#include "Enemy.h"
class ElectricBat :
	public Enemy
{
private:
	static constexpr float HOR_DISTANCE = 120.0f;
	static constexpr float VER_DISTANCE = 50.0f;
	static constexpr float DETECT_CAPTAIN_DISTANCE = 90.0f;
	static constexpr float MOVING_SPEED = 110.0f;

	Captain *cap;

	void OnAction();
	bool OnState(State state);
public:
	ElectricBat(Vector2 spawnPos, Grid* grid, Captain *cap);
	void Update(float dt, const std::vector<GameObject*>& coObjects) override;
	void TakeDamage(int damage) override;
	void HandleCollisions(float dt, const std::vector<GameObject*>& coObjects);
};

