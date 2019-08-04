#include "pch.h"
#include "CaptainStanding.h"
#include "DynamiteNapalm.h"
#include "ElectricBat.h"

void CaptainStanding::Enter(Captain& cap, State fromState, Data&& data)
{
	cap.vel.x;
	cap.vel.y = 170.0f;
	isToSittingTackle = false;
}

Data CaptainStanding::Exit(Captain& cap, State toState)
{
	switch (toState)
	{
	case State::Captain_Sitting:
		data.Add(IS_TO_SIT_TACKLE, isToSittingTackle);
		break;
	}

	return std::move(data);
}

void CaptainStanding::OnKeyUp(Captain& cap, BYTE keyCode)
{
}

void CaptainStanding::OnKeyDown(Captain& cap, BYTE keyCode)
{
	auto kControlDir = cap.nx > 0 ? KeyControls::Right : KeyControls::Left;

	// if pressdown two time in a row in direction
	if (setting.Get(kControlDir) == keyCode &&
		cap.lastKeyDown == kControlDir && cap.lastKeyUp == kControlDir)
	{
		std::chrono::duration<float> duration = std::chrono::steady_clock::now() - cap.timeLastKeyDown;
		if (duration.count() < 0.17f && !isOnMovingLedge)
		{
			isToSittingTackle = true;
			cap.SetState(State::Captain_Sitting);
			return;
		}
	}
	else if (keyCode == setting.Get(KeyControls::Jump))
	{
		cap.SetState(State::Captain_Jumping);
	}
}

void CaptainStanding::Update(Captain& cap, float dt, const std::vector<GameObject*>& coObjects)
{
	if (!isOnMovingLedge)
	{
		cap.vel.x = 0.0f;
	}

	if (wnd.IsKeyPressed(setting.Get(KeyControls::Down)))
	{
		cap.SetState(State::Captain_Sitting);
		return;
	}
	else if (wnd.IsKeyPressed(setting.Get(KeyControls::Up)))
	{
		cap.SetState(State::Captain_CoverTop);
		return;
	}
	else if (wnd.IsKeyPressed(setting.Get(KeyControls::Attack)))
	{
		if (cap.shieldOn) {
			cap.SetState(State::Captain_Throwing);
			return;
		}
		else {
			cap.SetState(State::Captain_Punching);
			return;
		}
	}
	else
	{
		int dir = 0;
		if (wnd.IsKeyPressed(setting.Get(KeyControls::Left)))
		{
			dir--;
		}
		if (wnd.IsKeyPressed(setting.Get(KeyControls::Right)))
		{
			dir++;
			cap.nx = 1;
		}
		if (dir != 0)
		{
			cap.nx = dir;
			cap.SetState(State::Captain_Walking);
			return;
		}
	}

	HandleCollisions(cap, dt, coObjects);
	if (!isOnGround && !isOnMovingLedge)
		cap.SetState(State::Captain_Falling);
}

void CaptainStanding::HandleCollisions(Captain& cap, float dt, const std::vector<GameObject*>& coObjects)
{
	isOnGround = false;

	auto coEvents = CollisionDetector::CalcPotentialCollisions(cap, coObjects, dt);
	if (coEvents.size() == 0)
	{
		cap.pos.x += cap.vel.x * dt;
		cap.pos.y += cap.vel.y * dt;
		return;
	}
	isOnMovingLedge = false;

	float min_tx, min_ty, nx, ny;
	CollisionDetector::FilterCollisionEvents(coEvents, min_tx, min_ty, nx, ny);

	if (coEvents.size() == 0) return;

	cap.pos.x += min_tx * cap.vel.x * dt;
	cap.pos.y += min_ty * cap.vel.y * dt;

	for (auto& e : coEvents)
	{
		if (auto block = dynamic_cast<Block*>(e.pCoObj)) {

			switch (block->GetType())
			{
			case ClassId::RigidBlock:
			case ClassId::PassableLedge:
				isOnGround = true;
				break;

			case ClassId::DamageBlock:
				isOnGround = true;
				cap.CollideWithPassableObjects(dt, e);
				break;

			case ClassId::NextMap:
			case ClassId::Switch:
			case ClassId::Door:
				cap.CollideWithPassableObjects(dt, e);
				break;

			case ClassId::ClimbableBar:
				cap.CollideWithPassableObjects(dt, e);
				break;
			case ClassId::Water:
			default:
				AssertUnreachable();
			}
		}
		else if (auto spawner = dynamic_cast<Spawner*>(e.pCoObj))
		{
			spawner->OnCollideWithCap(&cap);
			cap.CollideWithPassableObjects(dt, e); // go the remaining distance
		}
		else if (auto ambush = dynamic_cast<AmbushTrigger*>(e.pCoObj))
		{
			ambush->OnCollideWithCap(&cap);
			cap.CollideWithPassableObjects(dt, e);
		}
		else if (auto movingLedge = dynamic_cast<MovingLedge*>(e.pCoObj))
		{
			if (e.ny < 0)
			{
				isOnGround = true;
				isOnMovingLedge = true;
				cap.vel = movingLedge->GetVelocity();
				cap.vel.y += GRAVITY; // to make Captain and moving ledge still collide
			}
		}
		else if (auto breakableLedge = dynamic_cast<BreakableLedge*>(e.pCoObj))
		{
			isOnGround = true;
		}
		else if (dynamic_cast<Capsule*>(e.pCoObj)) {
			cap.CollideWithPassableObjects(dt, e);
		}
		else if (auto item = dynamic_cast<Item*>(e.pCoObj))
		{
			item->BeingCollected();
			cap.CollideWithPassableObjects(dt, e);
		}
		else if (auto enemy = dynamic_cast<Enemy*>(e.pCoObj))
		{
			if (!cap.isFlashing)
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
				else if (auto bat = dynamic_cast<ElectricBat*>(e.pCoObj))
				{
					if (bat->GetState() == State::ElectricBat_FlyAttack)
					{
						cap.SetState(State::CaptainElectricShock);
						return;
					}
				}
				if (!isOnMovingLedge)
					cap.SetState(State::Captain_Injured);
			}
			else {
				cap.CollideWithPassableObjects(dt, e);
			}
		}
		else if (auto bullet = dynamic_cast<Bullet*>(e.pCoObj)) {
			if (!cap.isFlashing)
			{
				cap.health.Subtract(bullet->GetDamage());
				cap.SetState(State::Captain_Injured);
			}
			else {
				cap.CollideWithPassableObjects(dt, e);
			}
		}
		else if (auto movingLedgeUpdater = dynamic_cast<MovingLedgeUpdater*>(e.pCoObj))
		{
			cap.CollideWithPassableObjects(dt, e);
		}
		else if (auto trap = dynamic_cast<ElectricTrap*>(e.pCoObj))
		{
			if (cap.curState != State::CaptainElectricShock && !cap.isFlashing&&trap->CanCauseElectricShock())
			{
				CaptainHealth::Instance().Set(0);
				cap.SetState(State::Captain_Injured);
				cap.CollideWithPassableObjects(dt, e);
			}
			else
				cap.CollideWithPassableObjects(dt, e);
		}

	}
}
