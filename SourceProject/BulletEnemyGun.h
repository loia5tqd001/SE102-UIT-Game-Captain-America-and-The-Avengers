#pragma once
#include "Bullet.h"

class BulletEnemyGun : public Bullet
{
private:
	static constexpr float BULLET_MOVING = 180.0f;

public:
	BulletEnemyGun(int nx, Enemy *enemy, const Vector2& spawnPos, Vector2 vel = { BULLET_MOVING, 0.0f });

	void Update(float dt, const std::vector<GameObject*>& coObjects) override;
	RectF GetBBox() const override;
	void Reflect();
};
