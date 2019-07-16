#include "pch.h"
#include "BulletEnemyGun.h"

BulletEnemyGun::BulletEnemyGun(const Vector2 & spawnPos, const Vector2 & vel, int nx) : 
	Bullet(State::BulletEnemyGun, 1, spawnPos, vel, nx)
{
	animations.emplace(State::BulletEnemyGun, Animation(SpriteId::BulletEnemyGun, 0.1f));
}

void BulletEnemyGun::Update(float dt, const std::vector<GameObject*>& coObjects)
{
	pos.x += vel.x * dt;
	pos.y += vel.y * dt;
}

