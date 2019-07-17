#include "pch.h"
#include "BulletEnemyGun.h"

BulletEnemyGun::BulletEnemyGun(int nx, const Vector2& spawnPos, Vector2 vel) :
	Bullet(State::BulletEnemyGun, 1, spawnPos, vel, nx)
{
	animations.emplace(State::BulletEnemyGun, Animation(SpriteId::BulletEnemyGun, 0.1f));
}

void BulletEnemyGun::Update(float dt, const std::vector<GameObject*>& coObjects)
{
	pos.x += vel.x * dt;
	pos.y += vel.y * dt;
	animations.at(curState).Update(dt);
}

RectF BulletEnemyGun::GetBBox() const
{
	return VisibleObject::GetBBox();
}
