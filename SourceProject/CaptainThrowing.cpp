#include "pch.h"
#include "CaptainThrowing.h"
#include "BulletEnemyRocket.h"



void CaptainThrowing::Enter(Captain& cap, State fromState, Data&& data)
{
	assert(fromState == State::Captain_Jumping || 
		   fromState == State::Captain_CoverTop || 
		   fromState == State::Captain_Walking ||
		   fromState == State::Captain_Standing);
	assert(cap.shieldOn);
	Sounds::PlayAt(SoundId::ShieldThrow);
	cap.vel.y = GRAVITY;
	cap.vel.x = 0;
	cap.shield->ThrowAway();
}

Data CaptainThrowing::Exit(Captain& cap, State toState)
{
	return Data{};
}

void CaptainThrowing::OnKeyUp(Captain& cap, BYTE keyCode)
{
}

void CaptainThrowing::OnKeyDown(Captain& cap, BYTE keyCode)
{
	if (keyCode == setting.Get(KeyControls::Jump))
	{
		cap.SetState(State::Captain_Jumping);
		return;
	}
}

void CaptainThrowing::Update(Captain& cap, float dt, const std::vector<GameObject*>& coObjects)
{
	HandleCollisions(cap, dt, coObjects);
	if (cap.animations.at(State::Captain_Throwing).IsDoneCycle()) {
		if (wnd.IsKeyPressed(setting.Get(KeyControls::Down)))
		{
			cap.SetState(State::Captain_Sitting);
			return;
		}
		if (wnd.IsKeyPressed(setting.Get(KeyControls::Up)))
		{
			cap.SetState(State::Captain_CoverTop);
			return;
		}
		cap.SetState(State::Captain_Standing);
	}
}

void CaptainThrowing::HandleCollisions(Captain& cap, float dt, const std::vector<GameObject*>& coObjects)
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
		if (auto bullet = dynamic_cast<Bullet*>(e.pCoObj))
		{
			if (!cap.isFlashing)
			{
				if (auto bullet = dynamic_cast<BulletEnemyRocket*>(e.pCoObj));
				cap.health.Subtract(bullet->GetDamage());
				bullet->HitCaptain();
				if (auto mini = dynamic_cast<DynamiteNapalm*>(e.pCoObj))
				{
					if (mini->CanCauseElectricShock())
					{
						cap.SetState(State::CaptainElectricShock);
						return;
					}
				}
				cap.SetState(State::Captain_Injured);
				//TODO: case BulletEnemyFlying
			}
		}
		else if (auto enemy = dynamic_cast<Enemy*>(e.pCoObj))
		{
			if (cap.isFlashing) { // undamagable
				cap.CollideWithPassableObjects(dt, e);
			}
			else {
				cap.health.Subtract(1);
				cap.SetState(State::Captain_Injured);
				enemy->TakeDamage(1);
			}
		}
		else if (auto block = dynamic_cast<Block*>(e.pCoObj))
		{
			if (block->GetType() == ClassId::DamageBlock)
			{
				cap.CollideWithPassableObjects(dt, e);
			}
		}
		else if (auto movingLedgeUpdater = dynamic_cast<MovingLedgeUpdater*>(e.pCoObj))
		{
			cap.CollideWithPassableObjects(dt, e);
		}
		else if (auto movingLedge = dynamic_cast<MovingLedge*>(e.pCoObj)) 
		{
			cap.vel = movingLedge->GetVelocity();
			cap.vel.y += GRAVITY; // to make Captain and moving ledge still collide
		}
		else if (auto spawner = dynamic_cast<Spawner*>(e.pCoObj))
		{
			spawner->OnCollideWithCap(&cap);
			cap.CollideWithPassableObjects(dt, e); // go the remaining distance
		}
		else if (auto trap = dynamic_cast<ElectricTrap*>(e.pCoObj))
		{
			cap.CollideWithPassableObjects(dt, e);
		}
		else if (auto door = dynamic_cast<Door*>(e.pCoObj))
		{
			cap.CollideWithPassableObjects(dt, e);
		}

	}
}

