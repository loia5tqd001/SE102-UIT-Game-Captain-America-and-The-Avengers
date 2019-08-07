#include "pch.h"
#include "CaptainInjured.h"

void CaptainInjured::HandleNoCollisions(Captain & cap, float dt)
{
	cap.pos.x += cap.vel.x *dt;
	cap.pos.y += cap.vel.y*dt;
}

void CaptainInjured::Enter(Captain& cap, State fromState, Data&& data)
{
	maxTimeHold = 0;
	pendingSwitchState = State::NotExist;
	DistanceLeftToClimb = 0.0f;
	cap.isFlashing = true;
	cap.vel.x = INJURE_FALL_SPEED * cap.nx*-1.0f;
	cap.vel.y = FALL_SPEED_VER;
	posxWhenGotInjure = cap.pos.x;
	posyWhenGotInjure = cap.pos.y;
	holdingDistance = 0.0f;

	if (fromState==State::Captain_Standing)
	{
		cap.vel.y += GRAVITY;
	}
}

Data CaptainInjured::Exit(Captain& cap, State toState)
{
	return std::move(data);
}

void CaptainInjured::OnKeyUp(Captain& cap, BYTE keyCode)
{
}

void CaptainInjured::OnKeyDown(Captain& cap, BYTE keyCode)
{
	if (keyCode == VK_NUMPAD9)
	{
		CaptainHealth::Instance().Set(0);
		pendingSwitchState = State::Captain_Dead;
	}
}

void CaptainInjured::Update(Captain& cap, float dt, const std::vector<GameObject*>& coObjects)
{
	//Health < 0 =>Die
	if (cap.health.Get() <= 0 && cap.curState != State::Captain_Dead)
	{
		pendingSwitchState = State::Captain_Dead;
	}

	cap.vel.x = INJURE_FALL_SPEED * cap.nx*-1.0f; // I don't know why but this fix being stuck in Injured
	HandleCollisions(cap, dt, coObjects);
}

void CaptainInjured::HandleCollisions(Captain& cap, float dt, const std::vector<GameObject*>& coObjects)
{
	holdingDistance += std::abs(cap.vel.x*dt);
	auto coEvents = CollisionDetector::CalcPotentialCollisions(cap, coObjects, dt);
	if (coEvents.size() == 0)
	{
		HandleNoCollisions(cap, dt);
		return;
	}

	float min_tx, min_ty, nx, ny;
	CollisionDetector::FilterCollisionEvents(coEvents, min_tx, min_ty, nx, ny);

	//Todo: Handle other Ledge

	if (coEvents.size() == 0) return;

	//Handle CaptainDead State
	if (pendingSwitchState == State::Captain_Dead)
	{
		for (auto& e : coEvents)
		{
			if (auto block = dynamic_cast<Block*>(e.pCoObj))
			{
				if (block->GetType() == ClassId::PassableLedge || block->GetType() == ClassId::RigidBlock)
				{
					cap.SetState(State::Captain_Dead);
					pendingSwitchState = State::NotExist;
					return;
				}
			}
		}
		HandleNoCollisions(cap, dt);
		return;
	}


	//Fall down Distance of Cap
	//if (cap.nx > 0)
	//{
	//	if (posxWhenGotInjure - cap.pos.x >= INJURE_DISTANCE)
	//	{
	//		cap.SetState(State::Captain_Standing);
	//		return;
	//	}
	//}
	//else if (cap.nx < 0)
	//{
	//	if (cap.pos.x - posxWhenGotInjure >= INJURE_DISTANCE)
	//	{
	//		cap.SetState(State::Captain_Standing);
	//		return;
	//	}
	//}

	//if (holdingDistance>INJURE_DISTANCE)
	//{
	//	cap.SetState(State::Captain_Standing);
	//	return;
	//}

	cap.pos.x += min_tx * cap.vel.x * dt;
	cap.pos.y += min_ty * cap.vel.y * dt;


	for (auto& e : coEvents)
	{
		if (auto spawner = dynamic_cast<Spawner*>(e.pCoObj))
		{
			spawner->OnCollideWithCap(&cap);
			cap.CollideWithPassableObjects(dt, e); // go the remaining distance
			return;
		}
		else if (auto ambush = dynamic_cast<AmbushTrigger*>(e.pCoObj))
		{
			//ambush->OnCollideWithCap();
			cap.CollideWithPassableObjects(dt, e);
			return;
		}
		else if (auto item = dynamic_cast<Item*>(e.pCoObj))
		{
			//item->BeingCollected();
			cap.CollideWithPassableObjects(dt, e);
			return;
		}
		else if (auto enemy = dynamic_cast<Enemy*>(e.pCoObj))
		{
			cap.CollideWithPassableObjects(dt, e); //Cap is flashing, immortal
			return;
		}
		else if (auto movingLedgeUpdater = dynamic_cast<MovingLedgeUpdater*>(e.pCoObj))
		{
			cap.CollideWithPassableObjects(dt, e);
		}
		else if (auto block = dynamic_cast<Block*>(e.pCoObj)) {

			switch (block->GetType())
			{
			case ClassId::PassableLedge:
				if (holdingDistance > INJURE_DISTANCE)
					cap.SetState(State::Captain_Standing);
				return;
				if (pendingSwitchState == State::Captain_Dead)
				{
					cap.SetState(State::Captain_Dead);
					return;
				}
				break;
			case ClassId::Water:
				if (e.ny != 0)
				{
					cap.SetState(State::Captain_FallToWater);
				}
				break;
				//Todo: Upgrade this
			case ClassId::ClimbableBar:
				data.Add(CLIMBBAR, block);
				cap.SetState(State::Captain_Climbing);
				cap.pos.y = block->GetPos().y;
				if (nx == -1 && cap.pos.x < block->GetPos().x)
				{
					cap.pos.x = block->GetPos().x;
				}
				else if (nx == 1 && cap.pos.x + cap.GetWidth() > block->GetPos().x + block->GetBBox().GetWidth())
				{
					cap.pos.x = block->GetPos().x + block->GetBBox().GetWidth() - cap.GetWidth();
				}
				cap.shield->UpdateByCapState(cap.curState, cap.pos);
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

				//Todo: check this 
			case ClassId::DamageBlock:
				cap.CollideWithPassableObjects(dt, e);
				break;
			case ClassId::RigidBlock:
				if (holdingDistance > INJURE_DISTANCE)
				{
					if (cap.pos.y - posyWhenGotInjure > 100.0f)
						cap.SetState(State::Captain_Sitting);
					else
						cap.SetState(State::Captain_Standing);
				}
				return;

			default:
				AssertUnreachable();
			}
		}
		else if (dynamic_cast<Capsule*>(e.pCoObj))
		{
			cap.CollideWithPassableObjects(dt, e);
			return;
		}
		else if (dynamic_cast<Bullet*>(e.pCoObj))
		{
			cap.CollideWithPassableObjects(dt, e);
			//return;
		}
		else if (auto movingLedge = dynamic_cast<MovingLedge*>(e.pCoObj))
		{
			if (e.ny < 0)
			{
				cap.SetState(State::Captain_Standing);
			}
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

