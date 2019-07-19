#pragma once
#include "Bullet.h"

class BulletEnemyWizard :
	public Bullet
{
private:
	static constexpr float BULLET_MOVING = 110.0f;

public:
	BulletEnemyWizard(int nx, const Vector2& spawnPos, Vector2& vel);

	void Update(float dt, const std::vector<GameObject*>& coObjects) override;
	RectF GetBBox() const override;
	static float GetXSpeed(){ return BULLET_MOVING; }
};


