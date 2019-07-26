#include "pch.h"
#include "BulletEnemyRocket.h"

BulletEnemyRocket::BulletEnemyRocket(int nx, int type, Enemy *enemy, const Vector2 & spawnPos, Vector2 vel) :
	Bullet((type == 1) ? State::BulletEnemyRocket_Cross : State::BulletEnemyRocket_Horizontal, 
		   1, spawnPos, { nx * BULLET_MOVING, 0.0f }, nx, enemy),
	type(type)
{
	if (type == 2) // EnemyRocket-BackAndForth
	{
		this->type = 0, this->vel.x *= 1.8f;
		animations.emplace(State::BulletEnemyRocket_Horizontal, Animation(SpriteId::BulletEnemyRocket_Horizontal, 0.05f));
	}
	else
	{
		animations.emplace(State::BulletEnemyRocket_Horizontal, Animation(SpriteId::BulletEnemyRocket_Horizontal, 0.1f));
		animations.emplace(State::BulletEnemyRocket_Cross, Animation(SpriteId::BulletEnemyRocket_Cross, 0.1f));
	}
	animations.emplace(State::Explode, Animation(SpriteId::Explode, 0.15f));

	if (nx < 0) GameObject::FlipPosXToLeft(pos.x, enemy->GetPosX(), this->GetWidth(), enemy->GetWidth()); // this code is critical
}
void BulletEnemyRocket::Update(float dt, const std::vector<GameObject*>& coObjects)
{
	if (animations.at(State::Explode).IsDoneCycle())
	{
		this->SetState(State::Destroyed);
		return;
	}
	animations.at(curState).Update(dt);
	pos.x += vel.x * dt;
	pos.y += vel.y * dt;
	if (type != 0)
	{
		if (abs(vel.y) <= abs(vel.x) / 2) this->SetState(State::BulletEnemyRocket_Horizontal);
		else this->SetState(State::BulletEnemyRocket_Cross);
		durationUp += GameTimer::Dt();
		if (durationUp >= 0.25)
		{
			vel.x = nx * BULLET_MOVING/sqrt(2);
			vel.y = -abs(vel.x);
		}
	}
}

void BulletEnemyRocket::HitCaptain()
{
	Sounds::PlayAt(SoundId::Explosion);
	this->SetState(State::Explode);
	vel = { 0.0f,0.0f };
}

