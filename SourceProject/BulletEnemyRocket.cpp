#include "pch.h"
#include "BulletEnemyRocket.h"

BulletEnemyRocket::BulletEnemyRocket(int nx, int type, Enemy *enemy, const Vector2 & spawnPos, Vector2 vel) :
	Bullet(State::BulletEnemyRocket_Horizontal, 1, spawnPos, { nx * BULLET_MOVING, 0.0f }, nx, enemy),
	type(type)
{
	animations.emplace(State::BulletEnemyRocket_Cross, Animation(SpriteId::BulletEnemyRocket_Cross, 0.1f));
	animations.emplace(State::BulletEnemyRocket_Horizontal, Animation(SpriteId::BulletEnemyRocket_Horizontal, 0.1f));

	if (nx < 0) GameObject::FlipPosXToLeft(pos.x, enemy->GetPosX(), this->GetWidth(), enemy->GetWidth()); // this code is critical
}
void BulletEnemyRocket::Update(float dt, const std::vector<GameObject*>& coObjects)
{
	pos.x += vel.x * dt;
	pos.y += vel.y * dt;
	if (type != 0)
	{
		if (abs(vel.y) > vel.x / 2) this->SetState(State::BulletEnemyRocket_Cross);
		if (abs(vel.y) < BULLET_MOVING / 5)
		{
			vel.y -= 0.4f;
			vel.x = std::sqrt(BULLET_MOVING*BULLET_MOVING - vel.y*vel.y);
		}
		else if (abs(vel.y) < BULLET_MOVING)
		{
			vel.y -= 0.8f;
			vel.x = std::sqrt(BULLET_MOVING*BULLET_MOVING - vel.y*vel.y);
		}
	}
}
