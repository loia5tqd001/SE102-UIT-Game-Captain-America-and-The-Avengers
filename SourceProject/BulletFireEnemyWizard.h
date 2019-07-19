#pragma once
#include "Bullet.h"
class BulletFireEnemyWizard :
	public Bullet
{
private:
	static constexpr float BULLET_MOVING = 110.0f;

public:
	BulletFireEnemyWizard(int nx, const Vector2& spawnPos, Vector2 vel = { BULLET_MOVING, 0.0f });

	void Update(float dt, const std::vector<GameObject*>& coObjects) override;
	RectF GetBBox() const override;
};

