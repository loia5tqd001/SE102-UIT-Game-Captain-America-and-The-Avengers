#include "pch.h"
#include "CaptainCoverLow.h"
#include "BulletEnemyRocket.h"


void CaptainCoverLow::Enter(Captain& cap, State fromState, Data&& data)
{
	assert(fromState == State::Captain_Spinning);
	cap.vel.y = GRAVITY;
	cap.vel.x = 0;
	isOnGround = false;
}

Data CaptainCoverLow::Exit(Captain& cap, State toState)
{
	return Data{};
}

void CaptainCoverLow::OnKeyUp(Captain& cap, BYTE keyCode)
{
	if (keyCode == setting.Get(KeyControls::Down))
	{
		if (!isOnGround)
			cap.SetState(State::Captain_Falling);
		else
			cap.SetState(State::Captain_Standing);
	}
}

void CaptainCoverLow::OnKeyDown(Captain& cap, BYTE keyCode)
{
	if (keyCode == setting.Get(KeyControls::Attack))
	{
		if (!isOnGround)
		{
			cap.SetState(State::Captain_Kicking); 
			return;
		}
		else
			cap.SetState(State::Captain_SitPunching);
	}
}

void CaptainCoverLow::Update(Captain& cap, float dt, const std::vector<GameObject*>& coObjects)
{
	HandleCollisions(cap, dt, coObjects);
	cap.vel.x = 0.0f;
	if (wnd.IsKeyPressed(setting.Get(KeyControls::Left)))
	{
		cap.nx = -1;
		if (!isOnGround)
			cap.vel.x = -MOVE_HOR;
		else
			cap.SetState(State::Captain_Walking);
	}
	if (wnd.IsKeyPressed(setting.Get(KeyControls::Right)))
	{
		cap.nx = 1;
		if (!isOnGround)
			cap.vel.x = MOVE_HOR;
		else
			cap.SetState(State::Captain_Walking);
	}
}

void CaptainCoverLow::HandleCollisions(Captain& cap, float dt, const std::vector<GameObject*>& coObjects)
{
	//isOnGround = false; We should do this, but it'll cause wrong sound, why?
	auto coEvents = CollisionDetector::CalcPotentialCollisions(cap, coObjects, dt);
	if (coEvents.size() == 0)
	{
		cap.pos.x += cap.vel.x * dt;
		cap.pos.y += cap.vel.y * dt;
		return;
	}

	float min_tx, min_ty, nx, ny;
	CollisionDetector::FilterCollisionEvents(coEvents, min_tx, min_ty, nx, ny);

	if (coEvents.size() == 0) return;

	cap.pos.x += min_tx * cap.vel.x * dt;
	cap.pos.y += min_ty * cap.vel.y * dt;

	for (auto& e : coEvents)
	{
		if (auto spawner = dynamic_cast<Spawner*>(e.pCoObj))
		{
			spawner->OnCollideWithCap(&cap);
			cap.CollideWithPassableObjects(dt, e); // go the remaining distance
		}
		if (auto bullet = dynamic_cast<Bullet*>(e.pCoObj))
		{
			if (!cap.isFlashing)
			{
				cap.health.Subtract(bullet->GetDamage());
				bullet->HitCaptain();
				cap.SetState(State::Captain_Injured);
			}
		}
		else if (auto ambush = dynamic_cast<AmbushTrigger*>(e.pCoObj))
		{
			// TODO: cover low mean can't collide with ambush trigger
			ambush->OnCollideWithCap(&cap);
			cap.CollideWithPassableObjects(dt, e);
		}
		else if (auto item = dynamic_cast<Item*>(e.pCoObj))
		{
			item->BeingCollected();
			cap.CollideWithPassableObjects(dt, e);
		}
		else if (auto enemy = dynamic_cast<Enemy*>(e.pCoObj))
		{
			if (cap.isFlashing) { // undamagable
				cap.CollideWithPassableObjects(dt, e);
			}
			else {
				if (isOnGround)
				{				
					cap.health.Subtract(1);
					cap.SetState(State::Captain_Injured);
					enemy->TakeDamage(1);
				}
				else {
					if (e.ny < 0 || e.nx != 0)
					{
						enemy->TakeDamage(3);
					}
					else
					{
						cap.health.Subtract(1);
						cap.SetState(State::Captain_Injured);
						enemy->TakeDamage(1);
					}
				}
			}
		}
		else if (auto block = dynamic_cast<Block*>(e.pCoObj)) {

			switch (block->GetType())
			{
			case ClassId::Water:
				if (e.ny < 0) {
					cap.vel.x = 0;
					//isOnGround = true; // will cause stand on the water
					Sounds::PlayAt(SoundId::Water);
				};
				break;

			case ClassId::NextMap:
				if (sceneManager.GetCurScene().canGoNextMap)
					sceneManager.GoNextScene();
				else cap.CollideWithPassableObjects(dt, e);
				break;

			case ClassId::Switch:
			case ClassId::Door:
				cap.CollideWithPassableObjects(dt, e);
				break;

			case ClassId::ClimbableBar:
				if (e.ny < 0)
					cap.SetState(State::Captain_Climbing);
				break;

			case ClassId::DamageBlock:
				if (e.ny < 0) isOnGround = true;
				cap.health.Subtract(1);
				cap.SetState(State::Captain_Injured);
				break;

			case ClassId::PassableLedge:
				if (e.ny < 0) {
					cap.vel.x = 0;
					if (!isOnGround) Sounds::PlayAt(SoundId::ShieldCollide);
					isOnGround = true;
				}
				break;
			case ClassId::RigidBlock:
				if (e.ny < 0) {
					cap.vel.x = 0;
					if (!isOnGround) Sounds::PlayAt(SoundId::ShieldCollide);
					isOnGround = true;
				}
				break;

			default:
				AssertUnreachable();
			}
		}
		else if (dynamic_cast<Capsule*>(e.pCoObj)) {
			cap.CollideWithPassableObjects(dt, e);
		}
	}
}


