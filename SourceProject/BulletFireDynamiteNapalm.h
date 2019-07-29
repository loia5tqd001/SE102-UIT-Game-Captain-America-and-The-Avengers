#pragma once
#include "Bullet.h"
class BulletFireDynamiteNapalm : public Bullet
{
private:
	static constexpr float BULLET_MOVING = 250.0f;
public:
	BulletFireDynamiteNapalm(int nx, Enemy *enemy, const Vector2& spawnPos, Vector2 vel = { BULLET_MOVING, 0.0f });
	void Update(float dt, const std::vector<GameObject*>& coObjects) override;
	RectF GetBBox() const override;
};

