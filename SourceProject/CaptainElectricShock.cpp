#include "pch.h"
#include "CaptainElectricShock.h"


void CaptainElectricShock::Enter(Captain& cap, State fromState, Data&& data)
{
	if (cap.health.Get() <= 0) 
		cap.SetState(State::Captain_Dead);
	cap.isFlashing = true;
	cap.vel.x = 0.0f;
	cap.vel.y = 100.0f;
}

Data CaptainElectricShock::Exit(Captain& cap, State toState)
{
	return Data{};
}

void CaptainElectricShock::OnKeyUp(Captain& cap, BYTE keyCode)
{
}

void CaptainElectricShock::OnKeyDown(Captain& cap, BYTE keyCode)
{
}

void CaptainElectricShock::Update(Captain& cap, float dt, const std::vector<GameObject*>& coObjects)
{
	if (cap.animations.at(cap.curState).IsDoneCycle())
	{
		cap.SetState(State::Captain_Standing);
		return;
	}
	HandleCollisions(cap, dt, coObjects);
}

void CaptainElectricShock::HandleCollisions(Captain& cap, float dt, const std::vector<GameObject*>& coObjects)
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
		if (auto block = dynamic_cast<Block*>(e.pCoObj)) 
		{

			switch (block->GetType())
			{
			case ClassId::RigidBlock:
			case ClassId::PassableLedge:
				break;
			case ClassId::DamageBlock:
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
		else if (auto enemy = dynamic_cast<Enemy*>(e.pCoObj))
		{
			cap.CollideWithPassableObjects(dt, e);
		}
		else if (auto bullet = dynamic_cast<Bullet*>(e.pCoObj))
		{
			cap.CollideWithPassableObjects(dt, e);
		}
	}
}

