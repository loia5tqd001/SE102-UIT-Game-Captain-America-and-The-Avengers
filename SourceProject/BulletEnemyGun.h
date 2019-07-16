#pragma once
#include "Bullet.h"

class BulletEnemyGun : public Bullet
{
private:
	static constexpr float BULLET_MOVING = 800.0f;

public:
	BulletEnemyGun(const Vector2& spawnPos, const Vector2& vel, int nx);

	void Update(float dt, const std::vector<GameObject*>& coObjects) override;
};
