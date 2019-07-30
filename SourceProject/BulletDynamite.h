#pragma once
#include "Bullet.h"
class BulletDynamite : public Bullet
{
private:
	static constexpr float pi = 3.1415926535898f;
	static constexpr float BULLET_MOVING = 0.0f;
	static constexpr float TIME_TO_THROW = 1.5f;
	static constexpr float Gravity = 900.0f;

	float Alpha = 0.0f;
	float Speed = 0.0f;
	bool triggered = false;
	float holdtime = 0.0f;

	inline void Target(bool isFar);
public:
	BulletDynamite(int nx, Enemy *enemy, const Vector2& spawnPos, Vector2 vel = { BULLET_MOVING, 0.0f }, bool farTarget = true);
	void Update(float dt, const std::vector<GameObject*>& coObjects) override;
	RectF GetBBox() const override;
	void HandleCollisions(float dt, const std::vector<GameObject*>& coObjects);
	void Trigger();
	void HitCaptain() override;

};

