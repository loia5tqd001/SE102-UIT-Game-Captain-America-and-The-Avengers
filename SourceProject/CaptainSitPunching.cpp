#include "pch.h"
#include "CaptainSitPunching.h"
#include "BulletEnemyRocket.h"



void CaptainSitPunching::Enter(Captain& cap, State fromState, Data&& data)
{
	assert(fromState == State::Captain_Sitting || fromState == State::Captain_CoverLow);
	cap.vel.y = GRAVITY;
	cap.vel.x = 0;

	// adjust position due to difference between sitting witdh and sitpunching width
	auto fromStateWidth = cap.animations.at(fromState).GetFrameSize().GetWidth();
	auto sitpunchingWidth = cap.animations.at(State::Captain_SitPunching).GetFrameSize().GetWidth();
	dentaX = (float(fromStateWidth) - float(sitpunchingWidth)) / 2.0f;
	if (cap.nx < 0) {
		cap.pos.x += dentaX;
	}
	else {
		cap.pos.x -= dentaX;
	}
}

Data CaptainSitPunching::Exit(Captain& cap, State toState)
{
	Sounds::PlayAt(SoundId::Punch);
	if (toState == State::Captain_Sitting)
	{
		if (cap.nx < 0) {
			cap.pos.x -= dentaX;
		}
		else {
			cap.pos.x += dentaX;
		}
	}
	return Data{};
}

void CaptainSitPunching::OnKeyUp(Captain& cap, BYTE keyCode)
{
}

void CaptainSitPunching::OnKeyDown(Captain& cap, BYTE keyCode)
{

}

void CaptainSitPunching::Update(Captain& cap, float dt, const std::vector<GameObject*>& coObjects)
{
	HandleCollisions(cap, dt, coObjects);
	if (cap.animations.at(cap.curState).IsDoneCycle()) {

		cap.SetState(State::Captain_Sitting); //move to Captain_Sitting, Captain_Walking should handle nx, KeyControls press, up and down
	}
}

void CaptainSitPunching::HandleCollisions(Captain& cap, float dt, const std::vector<GameObject*>& coObjects)
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
			else {
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
					enemy->TakeDamage(1);
					cap.health.Subtract(1);
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

