#include "pch.h"
#include "EnemyRocketBullet.h"

EnemyRocketBullet::EnemyRocketBullet(const Vector2 & spawnPos, const Vector2 & vel) : VisibleObject(State::EnemyGunBullet, spawnPos, vel)
{
	animations.emplace(State::EnemyRocketBullet_Cross, Animation(SpriteId::EnemyRocketBullet_Cross, 0.1f));
	animations.emplace(State::EnemyRocketBullet_Horizontal, Animation(SpriteId::EnemyRocketBullet_Horizontal, 0.1f));
}



