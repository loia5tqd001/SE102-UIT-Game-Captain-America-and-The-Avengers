#pragma once
#include "Counter.h"
#include "Enemy.h"
class EnemyRocket :
	public Enemy
{
private:
	static constexpr float WALKING_SPEED = 100.0f;
	static constexpr float JUMP_SPEED = 400.0f;
	static constexpr float GRAVITY = 1000.0f;
	static constexpr float FALL_BACK = 1000.0f;

	Grid* grid;
	bool onFlashing = false; //when take damage turn on flasing
	UINT health = 3;
	void HandleNoCollisions(float dt) override;
	void HandleCollisions(float dt, const std::vector<GameObject*>& coObjects) override;
public:
	EnemyRocket(const Vector2& spawnPos, const Vector2& vel, Grid *grid);

	RectF GetBBox() const override;
	void SetState(State state) override;
	void SpawnRocket(float cycle);

	void TakeDamage(UINT damage) override;
	void Update(float dt, const std::vector<GameObject*>& coObjects) override;
};

