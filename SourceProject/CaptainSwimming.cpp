#include "pch.h"
#include "CaptainSwimming.h"



void CaptainSwimming::Enter(Captain& cap, State fromState, Data&& data)
{
	cap.vel.x = WATER_FLOW_SPEED;
	cap.vel.y = 0;
}

Data CaptainSwimming::Exit(Captain& cap, State toState)
{
	return Data{};
}

void CaptainSwimming::OnKeyUp(Captain& cap, BYTE keyCode)
{
}

void CaptainSwimming::OnKeyDown(Captain& cap, BYTE keyCode)
{
	if (keyCode == setting.Get(KeyControls::Jump))
	{
		cap.SetState(State::Captain_Jumping);
		return;
	}
}

void CaptainSwimming::Update(Captain& cap, float dt, const std::vector<GameObject*>& coObjects)
{
	HandleCollisions(cap, dt, coObjects);

	auto animation = &cap.animations.at(cap.curState);
	animation->EraseCusFrameHoldTime(1);

	if (wnd.IsKeyPressed(setting.Get(KeyControls::Left)))
	{
		cap.vel.x = -CAP_SWIMMING_SPEED + WATER_FLOW_SPEED;
		cap.nx = -1;
	}
	else if (wnd.IsKeyPressed(setting.Get(KeyControls::Right)))
	{
		cap.vel.x = CAP_SWIMMING_SPEED + WATER_FLOW_SPEED;
		cap.nx = 1;
	}
	else if (wnd.IsKeyPressed(setting.Get(KeyControls::Down)))
	{
		cap.SetState(State::Captain_InWater);
	}
	else
	{
		cap.vel.x = WATER_FLOW_SPEED;
		animation->SetCusFrameHoldTime(1, std::numeric_limits<float>::infinity());
	}

	//cap.pos.x += cap.vel.x*dt;
}

void CaptainSwimming::HandleCollisions(Captain& cap, float dt, const std::vector<GameObject*>& coObjects)
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
					break;
				case ClassId::PassableLedge:
					cap.CollideWithPassableObjects(dt, e);
					break;

				case ClassId::DamageBlock:
				case ClassId::NextMap:
				case ClassId::Switch:
				case ClassId::Door:
				case ClassId::ClimbableBar:
				case ClassId::Water:
				default:
					AssertUnreachable(); // sitting is don't move
			}
		}
		if (auto spawner = dynamic_cast<Spawner*>(e.pCoObj))
		{
			spawner->OnCollideWithCap(&cap);
			cap.CollideWithPassableObjects(dt, e);
		}
		else if (auto ambush = dynamic_cast<AmbushTrigger*>(e.pCoObj))
		{
			ambush->OnCollideWithCap(&cap);
			cap.CollideWithPassableObjects(dt, e);
		}
		else if (auto item = dynamic_cast<Item*>(e.pCoObj))
		{
			item->BeingCollected();
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
	}
}
