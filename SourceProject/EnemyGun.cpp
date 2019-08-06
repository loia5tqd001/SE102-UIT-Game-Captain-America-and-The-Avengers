#include "pch.h"
#include "EnemyGun.h"
#include "Captain.h"
#include "BulletEnemyGun.h"
#include "CharlestonScene.h"

EnemyGun::EnemyGun(Behaviors behavior, Vector2 spawnPos, Captain* cap, Grid* grid) :
	Enemy(behavior, Data{}, State::EnemyGun_BeforeExplode, 2, spawnPos, grid),
	cap(cap)
{
	animations.emplace(State::EnemyGun_BeforeExplode, Animation(SpriteId::EnemyGun_BeforeExplode, 0.2f));
	groundPosY = spawnPos.y;
	if (behavior == Behaviors::EnemyGun_ShootFast) 
	{
		animations.emplace(State::EnemyGun_Stand, Animation(SpriteId::EnemyGun_Stand, 0.13f));
		animations.emplace(State::EnemyGun_Sitting, Animation(SpriteId::EnemyGun_Sitting, 0.5f));
	}
	else 
	{
		animations.emplace(State::EnemyGun_Stand, Animation(SpriteId::EnemyGun_Stand, 0.4f));
		animations.emplace(State::EnemyGun_Sitting, Animation(SpriteId::EnemyGun_Sitting, 1.5f));
	}
	if (behavior == Behaviors::EnemyGun_RunOnly) 
	{
		WALKING_SPEED *= 1.8f;
		animations.emplace(State::EnemyGun_Walking, Animation(SpriteId::EnemyGun_Walking, 0.09f));
	}
	else 
	{
		animations.emplace(State::EnemyGun_Walking, Animation(SpriteId::EnemyGun_Walking, 0.3f));
	}

	if (behavior == Behaviors::EnemyGun_Ambush) nx = 1;
	else nx = -cap->GetNx(); 

	switch (behavior)
	{
		case Behaviors::EnemyGun_ShootFast:
			SetState(State::EnemyGun_Sitting);
			break;

		case Behaviors::EnemyGun_Shoot:
			SetState(State::EnemyGun_Sitting);
			break;

		case Behaviors::EnemyGun_RunOnly:
			SetState(State::EnemyGun_Walking);
			break;

		case Behaviors::EnemyGun_Ambush:
			SetState(State::EnemyGun_Walking);
			break;
		default:
			AssertUnreachable();
	}
}

void EnemyGun::OnBehaviorShoot()
{
	assert(behavior == Behaviors::EnemyGun_Shoot ||
	       behavior == Behaviors::EnemyGun_ShootFast);

	// Sit -> Stand -> Shoot -> Stand -> Repeat
	nx = cap->GetPos().x > pos.x ? 1 : -1;
	if (animations.at(curState).IsDoneCycle())
	switch (curState)
	{
		case State::EnemyGun_Sitting:
			SetState(State::EnemyGun_Stand);
			break;

		case State::EnemyGun_Stand:
			if (!justShoot) {
				SpawnBullet();
				justShoot = true;
			}
			else {
				SetState(State::EnemyGun_Sitting);
				justShoot = false;
			}

		case State::EnemyGun_BeforeExplode:
		case State::Explode:
		case State::Destroyed:
			break;
		default:
			AssertUnreachable();
	}
}

void EnemyGun::OnBehaviorRunOnly()
{
	assert(behavior == Behaviors::EnemyGun_RunOnly);
	/*assert(curState == State::EnemyGun_Walking ||
	       curState == State::EnemyGun_BeforeExplode ||
	       curState == State::Explode ||
	       curState == State::Destroyed);*/
	if (!jumpOnce) {
		if (cap->IsShieldOn() == false && cap->getDirectionWhenThrow()*nx<0) {
			DogdeShield = true;
		}
		if (DogdeShield) {
			if (dynamic_cast<CharlestonScene*>(&SceneManager::Instance().GetCurScene())) {
				Jump(groundPosY, 60.0f);
			}
			else {
				Jump(groundPosY, 36.0f);
			}
		}
	}
}

void EnemyGun::OnBehaviorAmbush()
{
	assert(behavior == Behaviors::EnemyGun_Ambush);

	// Walking Right -> Stand&Shoot -> Walking Right-> Repeat 
	if (!jumpOnce) {
		if (cap->IsShieldOn() == false && cap->getDirectionWhenThrow()*nx < 0 && cap->GetPos().x>pos.x) {
			DogdeShield = true;
		}
		if (DogdeShield) {
			if (dynamic_cast<CharlestonScene*>(&SceneManager::Instance().GetCurScene())) {
				Jump(groundPosY, 60.0f);
			}
			else {
				Jump(groundPosY, 36.0f);
			}
		}
	}
	if (animations.at(curState).IsDoneCycle())
	switch (curState)
	{
		case State::EnemyGun_Walking:
			SetState(State::EnemyGun_Stand);
			break;

		case State::EnemyGun_Stand:
			if (!justShoot) {
				SpawnBullet();
				justShoot = true;
			}
			else {
				SetState(State::EnemyGun_Walking);
				justShoot = false;
			}

		case State::EnemyGun_BeforeExplode:
		case State::Explode:
		case State::Destroyed:
			break;
		default:
			AssertUnreachable();
	}
}
void EnemyGun::Jump(float posy, float height) //use this function in any height, just give it the ground pos and the height to jump
{
	if (health <= 0) return;
	static constexpr float JUMP_HOR = 55.0f;
	static constexpr float GRAVITY = 140.0f;

	VisibleObject::SetState(State::EnemyGun_Sitting);
	vel.x = nx*JUMP_HOR;

	if (behavior == Behaviors::EnemyGun_RunOnly)
		vel.x = nx * JUMP_HOR * 1.5;
	if (dirYJump == -1) // jump up
	{
		pos.y -= GRAVITY * GameTimer::Dt();// - accelerator;
		if (pos.y < posy - height) {
			dirYJump = 0;
		}
	}
	else if (dirYJump == 0) // kinda holding in the air
	{
		accelerator += 0.01f * GameTimer::Dt();
		pos.y += GRAVITY * GameTimer::Dt();
		if (pos.y > posy - height + 5) {
			dirYJump = 1; // falling
		}
	}
	else // falling
	{
		pos.y += GRAVITY * GameTimer::Dt() + accelerator;
		if (pos.y >= posy + 5) //why 9? it is the different in 2 sprite
		{
			pos.y = posy + 5;
			VisibleObject::SetState(State::EnemyGun_Walking);
			DogdeShield = false;
			jumpOnce = true;
			vel.x = nx * WALKING_SPEED;
		}
	}
	//pos.x += vel.x * 2 * GameTimer::Dt();
}

void EnemyGun::Update(float dt, const std::vector<GameObject*>& coObjects)
{
	UpdateAnimation(dt);
	if (curState == State::Explode) return;

	switch (behavior)
	{
		case Behaviors::EnemyGun_Shoot:
		case Behaviors::EnemyGun_ShootFast:
			OnBehaviorShoot();
			break;
		case Behaviors::EnemyGun_RunOnly:
			OnBehaviorRunOnly();
			break;
		case Behaviors::EnemyGun_Ambush:
			OnBehaviorAmbush();
			break;
		default:
			AssertUnreachable();
	}

	pos.x += vel.x*dt;
	pos.y += vel.y*dt;
}

void EnemyGun::SetState(State state)
{
	VisibleObject::SetState(state);

	switch (state)
	{
	case State::EnemyGun_Stand:
		vel.x = 0.0f;
		break;
	case State::EnemyGun_Walking:
		vel.x = nx * WALKING_SPEED;
		break;
	case State::EnemyGun_Sitting:
		vel.x = 0.0f;
		break;
	case State::EnemyGun_BeforeExplode:
		vel.x = -nx * FALL_BACK;
		break;
	case State::Explode:
		vel.x = 0;
		break;
	}
}

void EnemyGun::SpawnBullet() 
{
	if (isFlashing) return;
	if (curState == State::Explode || curState == State::EnemyGun_BeforeExplode) return;
	if (curState == State::EnemyGun_Sitting)
	{
		const auto bulletPos = pos + Vector2{ 24.0f, 11.0f };
		grid->SpawnObject(std::make_unique<BulletEnemyGun>(nx, this, bulletPos));
		Sounds::PlayAt(SoundId::BulletNormal);
	}
	else if (curState == State::EnemyGun_Stand)
	{
		const auto bulletPos = pos + Vector2{ 19.0f, 3.0f };
		grid->SpawnObject(std::make_unique<BulletEnemyGun>(nx, this, bulletPos));
		Sounds::PlayAt(SoundId::BulletNormal);
	}
}
