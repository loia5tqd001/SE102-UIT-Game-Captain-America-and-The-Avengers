#pragma once
#include "Enemy.h"
class ElectricTrap : public Enemy
{
private:
	static float holdtime;
	bool synced = false;
public:
	ElectricTrap(Vector2 spawnPos, Grid* grid, State initState = State::NotExist);
	void Update(float dt, const std::vector<GameObject*>& coObjects) override;
	void HandleCollisions(float dt, const std::vector<GameObject*>& coObjects);
	void PrecheckAABB(float dt, const std::vector<GameObject*>& coObjects);
	virtual void TakeDamage(int damage) override { return; }
};