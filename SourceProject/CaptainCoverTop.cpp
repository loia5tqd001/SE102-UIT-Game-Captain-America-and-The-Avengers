#include "pch.h"
#include "CaptainCoverTop.h"
#include "BulletEnemyRocket.h"


void CaptainCoverTop::Enter(Captain& cap, State fromState, Data&& data)
{
	assert(fromState == State::Captain_Standing || fromState == State::Captain_Throwing);
	cap.vel.x = 0;
	cap.vel.y = GRAVITY;
}

Data CaptainCoverTop::Exit(Captain& cap, State toState)
{
	return Data{};
}

void CaptainCoverTop::OnKeyUp(Captain& cap, BYTE keyCode)
{
	//if (keyCode == setting.Get(KeyControls::Up))
	//{
	//	cap.SetState(State::Captain_Standing);
	//}
}

void CaptainCoverTop::OnKeyDown(Captain& cap, BYTE keyCode)
{
	if (keyCode == setting.Get(KeyControls::Attack))
	{
		if (cap.shieldOn)
		{
			cap.SetState(State::Captain_Throwing);
			return;
		}
		else
		{
			cap.SetState(State::Captain_Punching);
			return;
		}
	}
	if (keyCode == setting.Get(KeyControls::Jump))
	{
		{
			cap.SetState(State::Captain_Jumping);
			return;
		}
	}
	if (keyCode == setting.Get(KeyControls::Left))
	{
		{
			cap.SetState(State::Captain_Walking);
			return; 
		}
	}
	if (keyCode == setting.Get(KeyControls::Right))
	{
		{
			cap.SetState(State::Captain_Walking);
			return;
		}
	}
	if (keyCode == setting.Get(KeyControls::Down))
	{
		{
			cap.SetState(State::Captain_Standing);
			return;
		}
	}
}

void CaptainCoverTop::Update(Captain& cap, float dt, const std::vector<GameObject*>& coObjects)
{
	HandleCollisions(cap, dt, coObjects);
	if (!wnd.IsKeyPressed(setting.Get(KeyControls::Up)))
	{
		cap.SetState(State::Captain_Standing);
	}
}

void CaptainCoverTop::HandleCollisions(Captain& cap, float dt, const std::vector<GameObject*>& coObjects)
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
				cap.SetState(State::Captain_Injured);
				//TODO: case BulletEnemyFlying
				//TODO: deflect some of the bullets from top
			}
		}
		else if (auto enemy = dynamic_cast<Enemy*>(e.pCoObj))
		{
			if (cap.isFlashing) { // undamagable
				cap.CollideWithPassableObjects(dt, e);
			}
			else 
			{
				cap.health.Subtract(1);
				enemy->TakeDamage(1);
				if (auto mini = dynamic_cast<DynamiteNapalm*>(e.pCoObj))
				{
					if (mini->CanCauseElectricShock())
					{
						cap.SetState(State::CaptainElectricShock);
						return;
					}
				}
				cap.SetState(State::Captain_Injured);
			}
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
	}
}


