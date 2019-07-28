#pragma once
#include "Bullet.h"
class BulletFireEnemyWizard :
	public Bullet
{
private:
	static constexpr float BULLET_MOVING = 230.0f;

public:
	BulletFireEnemyWizard(int nx, const Vector2& spawnPos, bool isVertical = 0, Enemy *enemy = nullptr);

	void Update(float dt, const std::vector<GameObject*>& coObjects) override;
	RectF GetBBox() const override;
};

