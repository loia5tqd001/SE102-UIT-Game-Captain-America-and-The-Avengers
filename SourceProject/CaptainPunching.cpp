#include "pch.h"
#include "CaptainPunching.h"
#include "BulletEnemyRocket.h"


void CaptainPunching::Enter(Captain& cap, State fromState, Data&& data)
{
	assert(!cap.shieldOn);
	assert(fromState == State::Captain_Standing || fromState == State::Captain_Walking || fromState == State::Captain_CoverTop);
	cap.vel.y = GRAVITY;
	cap.vel.x = 0;
}

Data CaptainPunching::Exit(Captain& cap, State toState)
{
	Sounds::PlayAt(SoundId::Punch);
	return Data{};
}

void CaptainPunching::OnKeyUp(Captain& cap, BYTE keyCode)
{
}

void CaptainPunching::OnKeyDown(Captain& cap, BYTE keyCode)
{
	if (keyCode == setting.Get(KeyControls::Jump))
	{
		cap.SetState(State::Captain_Jumping);
	}
}

void CaptainPunching::Update(Captain& cap, float dt, const std::vector<GameObject*>& coObjects)
{
	HandleCollisions(cap, dt, coObjects);
	if (cap.animations.at(cap.curState).IsDoneCycle()) {
		if (wnd.IsKeyPressed(setting.Get(KeyControls::Jump)))
		{
			cap.SetState(State::Captain_Jumping);
		}

		cap.SetState(State::Captain_Standing); //move to Captain_Walking, Captain_Walking should handle nx, KeyControls press, up and down
	}
}

void CaptainPunching::HandleCollisions(Captain& cap, float dt, const std::vector<GameObject*>& coObjects)
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
			}
		}
		else if (auto enemy = dynamic_cast<Enemy*>(e.pCoObj))
		{
			if (cap.isFlashing) { // undamagable and can not damage enemy
				cap.CollideWithPassableObjects(dt, e);
			}
			else 
			{
				if (nx * e.nx < 0)
				{
					enemy->TakeDamage(1);
					if (auto mini = dynamic_cast<DynamiteNapalm*>(e.pCoObj))
					{
						if (mini->CanCauseElectricShock())
						{
							cap.SetState(State::CaptainElectricShock);
							return;
						}
					}
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
	}
}


