#include "pch.h"
#include "BulletEnemyRocket.h"


BulletEnemyRocket::BulletEnemyRocket(const Vector2 & spawnPos, const Vector2 & vel, int nx) : 
	Bullet(State::BulletEnemyGun, 1, spawnPos, vel, nx)
{
	animations.emplace(State::BulletEnemyRocket_Cross, Animation(SpriteId::BulletEnemyRocket_Cross, 0.1f));
	animations.emplace(State::BulletEnemyRocket_Horizontal, Animation(SpriteId::BulletEnemyRocket_Horizontal, 0.1f));
}
