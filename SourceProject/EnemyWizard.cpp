#include "pch.h"
#include "EnemyWizard.h"
#include "BulletFireEnemyWizard.h"
#include "BulletEnemyWizard.h"

EnemyWizard::EnemyWizard(Behaviors behavior, const Data& behaviorData, const Vector2 & spawnPos, const Vector2 & vel, int nx, Grid * grid, Captain* captain)
	: Enemy(behavior, std::move(behaviorData),State::EnemyWizard_Stand, 2, spawnPos, grid)
{
	animations.emplace(State::EnemyWizard_BeforeDefeated, Animation(SpriteId::EnemyWizard_BeforeDefeated, 0.2f));
	animations.emplace(State::EnemyWizard_Defeated, Animation(SpriteId::EnemyWizard_Defeated, 0.25f));
	animations.emplace(State::EnemyWizard_DefeatedCaptain, Animation(SpriteId::EnemyWizard_DefeatedCaptain, 0.3f));
	animations.emplace(State::EnemyWizard_FlyDown, Animation(SpriteId::EnemyWizard_FlyDown, 0.6f));
	animations.emplace(State::EnemyWizard_FlyUp, Animation(SpriteId::EnemyWizard_FlyUp, 0.6f));
	animations.emplace(State::EnemyWizard_Flying, Animation(SpriteId::EnemyWizard_FlyDown, 0.6f));
	animations.emplace(State::EnemyWizard_ShootBullet, Animation(SpriteId::EnemyWizard_ShootBullet, 0.1f));
	animations.emplace(State::EnemyWizard_ShootBulletFire, Animation(SpriteId::EnemyWizard_ShootBulletFire, 0.1f));
	animations.emplace(State::EnemyWizard_ShootWhenFly, Animation(SpriteId::EnemyWizard_ShootWhenFly, 0.15f));
	animations.emplace(State::EnemyWizard_Stand, Animation(SpriteId::EnemyWizard_Stand, 0.3f));
	animations.emplace(State::EnemyWizard_Walking, Animation(SpriteId::EnemyWizard_Walking, 0.3f));

	beforeExplode = State::EnemyWizard_BeforeDefeated;
	Explode = State::EnemyWizard_Defeated;
	cap = captain;
}

void EnemyWizard::SpawnBullet()
{
	if (isFlashing) return;
	if (this->curState == State::EnemyWizard_ShootBullet) {
		const auto bulletPos = pos + Vector2{ 34.0f, 11.0f } *float(nx);

		//caculate vel.y
		Vector2 bulletVel;
		bulletVel.x = BulletEnemyWizard::GetXSpeed();
		bulletVel.y = std::abs(bulletPos.y - cap->GetPos().y) / std::abs(bulletPos.x - cap->GetPos().x) * bulletVel.x;

		grid->SpawnObject(std::make_unique<BulletEnemyWizard>(nx, bulletPos, bulletVel));
		Sounds::PlayAt(SoundId::BulletNormal);
	}
}

void EnemyWizard::SpawnBulletFire()
{
	if (isFlashing) return;
	if (this->curState == State::EnemyWizard_ShootWhenFly) {
		const auto bulletPos = pos + Vector2{ 23.0f, 31.0f } *float(nx);
		grid->SpawnObject(std::make_unique<BulletFireEnemyWizard>(1, bulletPos, true));
		Sounds::PlayAt(SoundId::BulletLazer);
	}
	if (this->curState == State::EnemyWizard_ShootBulletFire) {
		const auto bulletPos = pos + Vector2{ 34.0f, 11.0f } *float(nx);
		grid->SpawnObject(std::make_unique<BulletFireEnemyWizard>(nx, bulletPos, false));
		Sounds::PlayAt(SoundId::BulletLazer);
	}
}

void EnemyWizard::Update(float dt, const std::vector<GameObject*>& coObjects)
{
	//TODO: net nx base on cap pos
	//if(cap->GetPos().x > pos.x) nx = 1; else nx = -1; 

	//regular update
	pos.x += vel.x*dt;
	pos.y += vel.y*dt;
	//
	if (animations.at(State::EnemyWizard_ShootBullet).IsDoneCycle())
	{
		SetState(State::EnemyWizard_Stand);
	}
	else if (animations.at(State::EnemyWizard_ShootBulletFire).IsDoneCycle())
	{
		SetState(State::EnemyWizard_Stand);
	}
	else if (animations.at(State::EnemyWizard_ShootWhenFly).IsDoneCycle())
	{
		SetState(State::EnemyWizard_Flying);
	}

	//update animations
	UpdateAnimation(dt);
}

void EnemyWizard::SetState(State state)
{
	VisibleObject::SetState(state);

	switch (state)
	{
	case State::EnemyWizard_BeforeDefeated:
		vel.x = -nx * FALL_BACK; 
		vel.y = 0;
		break;
	case State::EnemyWizard_Defeated:
		vel.x = 0; 
		vel.x = 0;
		break;
	case State::EnemyWizard_FlyUp:
		vel.y = -FLYING_SPEED; 
		vel.x = 0;
		break;
	case State::EnemyWizard_FlyDown:
		vel.y = FLYING_SPEED;
		vel.x = 0;
		break;
	case State::EnemyWizard_Flying:
		vel.x = FLYING_SPEED * nx;
		vel.y = 0;
		break;
	case State::EnemyWizard_ShootBullet:
		vel.x = 0;
		vel.y = 0;
		break;
	case State::EnemyWizard_ShootBulletFire:
		vel.x = 0;
		vel.y = 0;
		break;
	case State::EnemyWizard_ShootWhenFly:
		break;
	case State::EnemyWizard_Stand:
		vel.x = 0;
		vel.y = 0;
		break;
	case State::EnemyWizard_Walking:
		vel.x = WALKING_SPEED*nx;
		vel.y = 0;
		break;
	}
}
static float groundPosY = 150; // testing ground pos.y
void EnemyWizard::testing(Window &win)
{	
	if (win.IsKeyPressed(VK_UP))
		this->SetState(State::EnemyWizard_FlyUp);
	else if (win.IsKeyPressed(VK_DOWN))
	{
		if (pos.y < groundPosY)
			this->SetState(State::EnemyWizard_FlyDown);
		else
			this->SetState(State::EnemyWizard_Stand);
	}
	else if (win.IsKeyPressed(VK_LEFT))
	{
		nx = -1;
		if (pos.y < groundPosY)
			this->SetState(State::EnemyWizard_Flying);
		else
			this->SetState(State::EnemyWizard_Walking);
	}
	else if (win.IsKeyPressed(VK_RIGHT))
	{
		nx = 1;
		if (pos.y < groundPosY)
			this->SetState(State::EnemyWizard_Flying);
		else
			this->SetState(State::EnemyWizard_Walking);
	}
	if (win.IsKeyPressed('A')) // in real data we shoot base on cap pos if shoot vertical
	{
		if (curState == State::EnemyWizard_Flying || curState == State::EnemyWizard_FlyUp || curState == State::EnemyWizard_FlyDown)
			this->SetState(State::EnemyWizard_ShootWhenFly);
		else
			this->SetState(State::EnemyWizard_ShootBullet);
		SpawnBulletFire();
	}
	else if (win.IsKeyPressed('S')) // we always shoot base on cap pos
	{
		if (curState != State::EnemyWizard_Flying && curState != State::EnemyWizard_FlyUp && curState != State::EnemyWizard_FlyDown)
			this->SetState(State::EnemyWizard_ShootBullet);
		SpawnBullet();
	}
	
}
