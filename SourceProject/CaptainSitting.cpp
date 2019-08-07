#include "pch.h"
#include "CaptainSitting.h"


void CaptainSitting::Enter(Captain& cap, State fromState, Data&& data)
{
	cap.vel.x = 0.0f;
	cap.vel.y = JUMP_SPEED_VER;
	switch (fromState)
	{
		case State::Captain_Walking:
		case State::Captain_Standing:
			if (data.Count(IS_TO_SIT_TACKLE))
			{
				isSitToTackle = data.Get<bool>(IS_TO_SIT_TACKLE);
			}
			break;
		default:
			isSitToTackle = false;
			break;
	}

}

Data CaptainSitting::Exit(Captain& cap, State toState)
{
	return Data{};
}

void CaptainSitting::OnKeyUp(Captain& cap, BYTE keyCode)
{
	//if (setting.IsKeyControl(keyCode))
	//{
	//	cap.SetState(State::Captain_Standing);
	//}
}

void CaptainSitting::OnKeyDown(Captain& cap, BYTE keyCode)
{
	int dir = 0;
	if (keyCode == setting.Get(KeyControls::Left))
	{
		dir --;
	}
	else if (keyCode == setting.Get(KeyControls::Right))
	{
		dir ++;
	}
	else if (keyCode == setting.Get(KeyControls::Attack) && wnd.IsKeyPressed(setting.Get(KeyControls::Down)))
	{
		cap.SetState(State::Captain_SitPunching);
	}
	else if (keyCode == setting.Get(KeyControls::Jump) && !wnd.IsKeyPressed(setting.Get(KeyControls::Down)))
	{
		cap.SetState(State::Captain_Jumping);
	}

	if (dir != 0)
	{
		cap.nx = dir;
		cap.SetState(State::Captain_Walking);
	}
}

void CaptainSitting::Update(Captain& cap, float dt, const std::vector<GameObject*>& coObjects)
{
	HandleCollisions(cap, dt, coObjects);
	if (cap.animations.at(cap.curState).IsDoneCycle())
	{
		if (isSitToTackle)
		{
			cap.SetState(State::Captain_Tackle);
		}
		else if (!wnd.IsKeyPressed(setting.Get(KeyControls::Down)))
		{
			cap.SetState(State::Captain_Standing);
		}
	}
}

void CaptainSitting::HandleCollisions(Captain& cap, float dt, const std::vector<GameObject*>& coObjects)
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
		if (auto block = dynamic_cast<Block*>(e.pCoObj)) {

			switch (block->GetType())
			{
				case ClassId::RigidBlock:
					if (e.ny > 0) AssertUnreachable();
					break;
				case ClassId::PassableLedge:
					if (e.ny < 0)
					{
						if (wnd.IsKeyPressed(setting.Get(KeyControls::Jump)) && wnd.IsKeyPressed(setting.Get(KeyControls::Down)))
						{
							cap.SetState(State::Captain_Falling);
							cap.pos.y += 1.0f;
							cap.canPhaseThroughFloor = true;
							cap.phasingState = State::Captain_Falling;
						}
					}
					else
					{
						cap.CollideWithPassableObjects(dt, e);
					}
					break;

				case ClassId::DamageBlock:
					cap.CollideWithPassableObjects(dt, e);
					break;

				case ClassId::Switch:
					cap.CollideWithPassableObjects(dt, e);
					break;
				case ClassId::NextMap:
				case ClassId::Door:
				case ClassId::ClimbableBar:
				case ClassId::Water:
				default:
					break;
					//AssertUnreachable(); // sitting is don't move
			}
		}
		else if (auto spawner = dynamic_cast<Spawner*>(e.pCoObj))
		{
			spawner->OnCollideWithCap(&cap);
			cap.CollideWithPassableObjects(dt, e); // go the remaining distance
		}
		else if (auto ambush = dynamic_cast<AmbushTrigger*>(e.pCoObj))
		{
			//AssertUnreachable();
		}
		else if (auto ledge = dynamic_cast<MovingLedge*>(e.pCoObj))
		{
			if (e.ny < 0)
			{
				cap.vel = ledge->GetVelocity();
				cap.vel.y += GRAVITY; // to make Captain and moving ledge still collide
			}
		}
		else if (dynamic_cast<BreakableLedge*>(e.pCoObj))
		{
			return;
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
			enemy->TakeDamage(1);
			if (auto mini = dynamic_cast<DynamiteNapalm*>(e.pCoObj))
			{
				if (mini->CanCauseElectricShock())
				{
					cap.SetState(State::CaptainElectricShock);
					return;
				}
			}
			cap.CollideWithPassableObjects(dt, e);
		}
		else if (auto bullet = dynamic_cast<Bullet*>(e.pCoObj)) {
			if (!cap.isFlashing)
			{
				cap.SetState(State::Captain_Injured);
				cap.health.Subtract(bullet->GetDamage());
			}
			else {
				cap.CollideWithPassableObjects(dt, e);
			}
		}
		else if (auto movingLedgeUpdater = dynamic_cast<MovingLedgeUpdater*>(e.pCoObj))
		{
			cap.CollideWithPassableObjects(dt, e);
		}
		else if (auto door = dynamic_cast<Door*>(e.pCoObj))
		{
			cap.CollideWithPassableObjects(dt, e);
		}

	}
}


