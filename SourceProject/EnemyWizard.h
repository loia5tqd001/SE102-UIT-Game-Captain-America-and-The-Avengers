#pragma once
#include "Enemy.h"

class EnemyWizard : public Enemy
{
private:
	static constexpr float WALKING_SPEED = 80.0f;
	static constexpr float FLYING_SPEED = 150.0f;
	static constexpr float FALL_BACK = 70.0f;
	static constexpr float GROUND = 145.0f;
	static constexpr float ROOF = 10.0f;
	static constexpr float MAX_POS_X = 210.0f;
	static constexpr float MIN_POS_X = 20.0f;

	Captain& cap;
	bool Onbehaviors(Behaviors behavior);
public:
	EnemyWizard(Behaviors behavior, Data&& behaviorData, Vector2 spawnPos, Vector2 vel, int nx, Grid * grid, Captain& cap);
	void SetState(State state) override;
	void SpawnBullet();
	void Update(float dt, const std::vector<GameObject*>& coObjects) override;
	void SpawnBulletFire();
	void testing(Window &win); //only use for testing
	void HandleCollisions(float dt, const std::vector<GameObject*>& coObjects);
};

