#pragma once
#include "Counter.h"
#include "Enemy.h"

class EnemyGun :
	public Enemy
{
private:
	static constexpr float WALKING_SPEED      =  100.0f;
	static constexpr float JUMP_SPEED         =  400.0f;
	static constexpr float GRAVITY            = 1000.0f;
	static constexpr float FALL_BACK	      = 1000.0f;

	void HandleCollisions(float dt, const std::vector<GameObject*>& coObjects) override;
	void HandleNoCollisions(float dt) override;

public:
	EnemyGun(const Vector2& spawnPos, const Vector2& vel, Grid *grid);

	RectF GetBBox() const override;
	void SetState(State state) override;
	void SpawnBullet(float cycle);

	UINT GetHeight() const;
	void OnKneeHeight(UINT oldHeight); // recalculate position when lowing down body (height changed)
	void TakeDamage(UINT damage) override;
	void Update(float dt, const std::vector<GameObject*>& coObjects) override;
};

