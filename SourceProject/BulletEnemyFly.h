#pragma once
#include "Bullet.h"
class BulletEnemyFly :
	public Bullet
{
	static constexpr float BULLET_MOVING = 140.0f;
	static constexpr float TIME_FLY_STRAIGHT = 0.3f;
	static constexpr float MIN_CHANGE_VELX_PER_FRAME = 5.0f;
	static constexpr float MAX_TIME_TILL_EXPLODE = 2.5f;
	static constexpr float MIN_TIME_TILL_EXPLODE = 1.7f;
	Captain *cap;
	float counterExplode = 0.0f;
	float counterTimeFlyStraight = 0.0f;
	void SetAnimationByVel();
	void CalculateVelByCapPos(float capx, float capy);
	void UpdateVelYbyVelX(float velx);
public:
	BulletEnemyFly(int nx, Enemy *enemy, const Vector2 & spawnPos, Captain *cap);
	void Update(float dt, const std::vector<GameObject*>& coObjects);
	void HitCaptain() override;
};

