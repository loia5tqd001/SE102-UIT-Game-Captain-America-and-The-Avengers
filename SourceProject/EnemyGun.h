#pragma once
#include "Counter.h"

class EnemyGun :
	public VisibleObject
{
private:
	static constexpr float WALKING_SPEED      =  100.0f;
	static constexpr float JUMP_SPEED         =  400.0f;
	static constexpr float GRAVITY            = 1000.0f;
	static constexpr float FALL_BACK	      = 1000.0f;

	Counter counter;
	Grid* grid;
public:
	EnemyGun(const Vector2& spawnPos, const Vector2& vel, Grid *grid);

	RectF GetBBox() const override;
	void SetState(State state) override;
	void SpawnBullet(float cycle);

	UINT GetHeight() const;
	void OnKneeHeight(UINT oldHeight); // recalculate position when lowing down body (height changed)

	void Update(float dt, const std::vector<GameObject*>& coObjects) override;
	void HandleNoCollisions(float dt);
	void HandleCollisions(float dt, const std::vector<GameObject*>& coObjects);
};

