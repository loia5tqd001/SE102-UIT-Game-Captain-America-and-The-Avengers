#pragma once
#include "Bullet.h"

class BulletEnemyWizard :
	public Bullet
{
private:
	static constexpr float BULLET_MOVING = 150.0f;
	static constexpr float GRAVITY = 15.0f;
	static constexpr float GROUND = 195.0f;

	bool isReflected = false;
	float holdtime = 0;

public:
	BulletEnemyWizard(int nx, const Vector2& spawnPos, Vector2& vel, Enemy *enemy);

	void Update(float dt, const std::vector<GameObject*>& coObjects) override;
	RectF GetBBox() const override;
	static float GetXSpeed(){ return BULLET_MOVING; }
	void Reflect();
};


