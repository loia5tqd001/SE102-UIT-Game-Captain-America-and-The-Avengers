#include "pch.h"
#include "EnemyWizard.h"

EnemyWizard::EnemyWizard(const Vector2 & spawnPos, const Vector2 & vel, int nx, Grid * grid) :
	Enemy(State::EnemyGun_Stand, 15, spawnPos, vel, nx, grid)
{
	animations.emplace(State::EnemyWizard_BeforeDefeated, Animation(SpriteId::EnemyWizard_BeforeDefeated, 0.1f));
	animations.emplace(State::EnemyWizard_Defeated, Animation(SpriteId::EnemyWizard_Defeated, 0.1f));
	animations.emplace(State::EnemyWizard_DefeatedCaptain, Animation(SpriteId::EnemyWizard_DefeatedCaptain, 0.3f));
	animations.emplace(State::EnemyWizard_FlyDown, Animation(SpriteId::EnemyWizard_FlyDown, 0.1f));
	animations.emplace(State::EnemyWizard_FlyUp, Animation(SpriteId::EnemyWizard_FlyUp, 0.25f));
	animations.emplace(State::EnemyWizard_ShootBullet, Animation(SpriteId::EnemyWizard_ShootBullet, 0.1f));
	animations.emplace(State::EnemyWizard_ShootBulletFire, Animation(SpriteId::EnemyWizard_ShootBulletFire, 0.1f));
	animations.emplace(State::EnemyWizard_ShootWhenFly, Animation(SpriteId::EnemyWizard_ShootWhenFly, 0.3f));
	animations.emplace(State::EnemyWizard_Stand, Animation(SpriteId::EnemyWizard_Stand, 0.3f));
	animations.emplace(State::EnemyWizard_Walking, Animation(SpriteId::EnemyWizard_Walking, 0.3f));

	beforeExplode = State::EnemyGun_BeforeExplode;
}
