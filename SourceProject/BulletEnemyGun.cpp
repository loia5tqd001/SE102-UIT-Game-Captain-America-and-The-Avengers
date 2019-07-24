#include "pch.h"
#include "BulletEnemyGun.h"

BulletEnemyGun::BulletEnemyGun(int nx, Enemy *enemy, const Vector2& spawnPos, Vector2 vel) :
	Bullet(State::BulletEnemyGun, 1, spawnPos, { nx * BULLET_MOVING, 0.0f }, nx, enemy)
{
	animations.emplace(State::BulletEnemyGun, Animation(SpriteId::BulletEnemyGun, 0.1f));

	if (nx < 0) GameObject::FlipPosXToLeft(pos.x, enemy->GetPosX(), this->GetWidth(), enemy->GetWidth()); // this code is critical
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

void BulletEnemyGun::Reflect()
{
	vel.x = -vel.x;
	vel.y = -std::abs(vel.x);
}

