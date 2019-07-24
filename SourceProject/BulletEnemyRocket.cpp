#include "pch.h"
#include "BulletEnemyRocket.h"

BulletEnemyRocket::BulletEnemyRocket(int nx, int type, Enemy *enemy, const Vector2 & spawnPos, Vector2 vel) :
	Bullet((type == 1) ? State::BulletEnemyRocket_Cross : State::BulletEnemyRocket_Horizontal, 
		   1, spawnPos, { nx * BULLET_MOVING, 0.0f }, nx, enemy),
	type(type)
{
	if (type == 2) // EnemyRocket-BackAndForth
	{
		this->type = 0, this->vel.x *= 2;
		animations.emplace(State::BulletEnemyRocket_Horizontal, Animation(SpriteId::BulletEnemyRocket_Horizontal, 0.05f));
	}
	else
	{
		animations.emplace(State::BulletEnemyRocket_Horizontal, Animation(SpriteId::BulletEnemyRocket_Horizontal, 0.1f));
		animations.emplace(State::BulletEnemyRocket_Cross, Animation(SpriteId::BulletEnemyRocket_Cross, 0.1f));
	}

	if (nx < 0) GameObject::FlipPosXToLeft(pos.x, enemy->GetPosX(), this->GetWidth(), enemy->GetWidth()); // this code is critical
}
void BulletEnemyRocket::Update(float dt, const std::vector<GameObject*>& coObjects)
{
	animations.at(curState).Update(dt);
	pos.x += vel.x * dt;
	pos.y += vel.y * dt;
	if (type != 0)
	{
		if (abs(vel.y) < abs(vel.x) / 2) this->SetState(State::BulletEnemyRocket_Horizontal);
		else this->SetState(State::BulletEnemyRocket_Cross);
		if (abs(vel.y) < BULLET_MOVING / 5)
		{
			vel.y -= 0.5;
			vel.x = nx * std::sqrt(BULLET_MOVING*BULLET_MOVING - vel.y*vel.y);
		}
		else if (abs(vel.y) < BULLET_MOVING / 4)
		{
			vel.y -= 0.6f;
			vel.x = nx * std::sqrt(BULLET_MOVING*BULLET_MOVING - vel.y*vel.y);
		}
		else if (abs(vel.y) < BULLET_MOVING / 3)
		{
			vel.y -= 0.7f;
			vel.x = nx * std::sqrt(BULLET_MOVING*BULLET_MOVING - vel.y*vel.y);
		}
		else if (abs(vel.y) < BULLET_MOVING / 2)
		{
			vel.y -= 0.8f;
			vel.x = nx * std::sqrt(BULLET_MOVING*BULLET_MOVING - vel.y*vel.y);
		}
		else if (abs(vel.y) < BULLET_MOVING)
		{
			vel.y -= 0.9f;
			vel.x = nx * std::sqrt(BULLET_MOVING*BULLET_MOVING - vel.y*vel.y);
		}
	}
}
