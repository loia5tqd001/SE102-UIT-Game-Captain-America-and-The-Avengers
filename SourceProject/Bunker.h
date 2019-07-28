#pragma once
#include "Enemy.h"

class Bunker : public Enemy
{
private:
	int lastIdleState;
	int countSpin = 7;

	void SpawnBullet();
public:
	Bunker(State initState, Vector2 spawnPos, Grid* grid);
	void TakeDamage(int damage) override; // call when captain hit bunker
	void Update(float dt, const std::vector<GameObject*>& coObjects = {}) override;
};

