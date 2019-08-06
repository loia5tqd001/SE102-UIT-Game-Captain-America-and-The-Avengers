#pragma once
#include "Counter.h"
#include "Enemy.h"

class EnemyGun : public Enemy
{
private:
	static constexpr float JUMP_SPEED    = 50.0f;

	float WALKING_SPEED = 55.0f;
	bool justShoot = false;
	Captain* cap;
	int dirYJump = -1;
	float accelerator = 0.2f;
	bool DogdeShield = false;
	bool jumpOnce = false;
	float groundPosY = 0;

	void OnBehaviorShoot();
	void OnBehaviorRunOnly();
	void OnBehaviorAmbush();
	void Jump(float posy, float height);
public:
	EnemyGun(Behaviors behavior, Vector2 spawnPos, Captain* cap, Grid* grid);

	void SetState(State state) override;
	void SpawnBullet();
	
	void Update(float dt, const std::vector<GameObject*>& coObjects) override;
};

