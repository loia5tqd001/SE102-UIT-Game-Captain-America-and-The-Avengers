#include "pch.h"
#include "ElectricTrap.h"


ElectricTrap::ElectricTrap(Vector2 spawnPos, Grid * grid, State initState):
Enemy(Behaviors::NotExists, Data{}, State::NotExist, std::numeric_limits<int>::infinity(), spawnPos, grid,initState)
{
	animations.emplace(State::Invisible, Animation(SpriteId::Invisible, 2.0f));
	animations.emplace(State::ElectricTrap_Active, Animation(SpriteId::ElectricTrap, 0.1f));
}

void ElectricTrap::Update(float dt, const std::vector<GameObject*>& coObjects)
{
	if (!synced)
	{
		for (auto& o : coObjects)
		{
			if (auto otherTrap = dynamic_cast<ElectricTrap*>(o))
			{
				if (otherTrap!=this)
				{
					otherTrap->animations.at(curState).Reset();
					synced = true;
					break;
				}
			}
		}
	}
	PrecheckAABB(dt, coObjects);
	HandleCollisions(dt, coObjects);
	animations.at(curState).Update(dt);
}

void ElectricTrap::HandleCollisions(float dt, const std::vector<GameObject*>& coObjects)
{
	auto coEvents = CollisionDetector::CalcPotentialCollisions(*this, coObjects, dt);
	if (coEvents.size() == 0)
	{
		return;
	}

	float min_tx, min_ty, nx, ny;
	CollisionDetector::FilterCollisionEvents(coEvents, min_tx, min_ty, nx, ny);

	if (coEvents.size() == 0)
		return;

	for (auto&e : coEvents)
	{
		if (auto cap = dynamic_cast<Captain*>(e.pCoObj))
		{
			if (curState==State::ElectricTrap_Active && cap->GetState()!=State::CaptainElectricShock)
			{
				cap->SetState(State::CaptainElectricShock);
			}
			return;
		}
	}
}

void ElectricTrap::PrecheckAABB(float dt, const std::vector<GameObject*>& coObjects)
{
	auto trapBox = this->GetBBox();
	
	for (auto &o : coObjects)
	{
		if (auto cap = dynamic_cast<Captain*>(o))
		{
			if (!trapBox.IsIntersect(o->GetBBox()))
				return;
			if (curState == State::ElectricTrap_Active && cap->GetState() != State::CaptainElectricShock)
				cap->SetState(State::CaptainElectricShock);
			return;
		}
	}
}
