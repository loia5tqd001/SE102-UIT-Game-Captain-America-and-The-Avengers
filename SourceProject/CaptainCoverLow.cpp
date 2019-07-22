#include "pch.h"
#include "CaptainCoverLow.h"
#include "BulletEnemyRocket.h"


void CaptainCoverLow::Enter(Captain& cap, State fromState, Data&& data)
{
	assert(fromState == State::Captain_Spinning);
	cap.vel.y = GRAVITY;
	cap.vel.x = 0;
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
			cap.SetState(State::Captain_Jumping);
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
	if (keyCode == setting.Get(KeyControls::Left))
	{
		cap.SetState(State::Captain_Walking);
	}
	if (keyCode == setting.Get(KeyControls::Right))
	{
		cap.SetState(State::Captain_Walking);
	}
	if (keyCode == setting.Get(KeyControls::Up))
	{
		cap.SetState(State::Captain_Standing);
	}
}

void CaptainCoverLow::Update(Captain& cap, float dt, const std::vector<GameObject*>& coObjects)
{
	if (wnd.IsKeyPressed(setting.Get(KeyControls::Left)))
	{
		if (!isOnGround)
			cap.vel.x = -MOVE_HOR;
	}
	if (wnd.IsKeyPressed(setting.Get(KeyControls::Right)))
	{
		if (!isOnGround)
			cap.vel.x = MOVE_HOR;
	}
	HandleCollisions(cap, dt, coObjects);
}

void CaptainCoverLow::HandleCollisions(Captain& cap, float dt, const std::vector<GameObject*>& coObjects)
{
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
			spawner->OnCollideWithCap();
			cap.CollideWithPassableObjects(dt, e); // go the remaining distance
		}
		if (auto bullet = dynamic_cast<Bullet*>(e.pCoObj))
		{
			if (!cap.isFlashing)
			{
				if (auto bullet = dynamic_cast<BulletEnemyRocket*>(e.pCoObj));
				cap.health.Subtract(bullet->GetDamage());
				cap.SetState(State::Captain_Injured);
				//TODO: case BulletEnemyFlying
				//TODO: deflect some of the bullets from below
			}
		}
		else if (auto ambush = dynamic_cast<AmbushTrigger*>(e.pCoObj))
		{
			// TODO: cover low mean can't collide with ambush trigger
			if (!ambush->IsActive())
				ambush->Active();
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
				if (e.ny < 0)
				{
					enemy->TakeDamage(3);
				}
				else
				{
					cap.SetState(State::Captain_Injured);
					enemy->TakeDamage(1);
				}
			}
		}
		else if (auto block = dynamic_cast<Block*>(e.pCoObj)) {

			switch (block->GetType())
			{
			case ClassId::Water:
				if (e.ny < 0) {};
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
				cap.SetState(State::Captain_Injured);
				break;

			case ClassId::PassableLedge:
			case ClassId::RigidBlock:
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


