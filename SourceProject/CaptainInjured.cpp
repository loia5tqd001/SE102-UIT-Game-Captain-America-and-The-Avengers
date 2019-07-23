#include "pch.h"
#include "CaptainInjured.h"

void CaptainInjured::HandleNoCollisions(Captain & cap, float dt)
{
	cap.pos.x += cap.vel.x *dt;
	cap.pos.y += cap.vel.y*dt;
}

void CaptainInjured::Enter(Captain& cap, State fromState, Data&& data)
{
	cap.isFlashing = true;
	cap.vel.x = INJURE_FALL_SPEED * cap.nx*-1.0f;
	cap.vel.y = FALL_SPEED_VER;
	posxWhenGotInjure = cap.pos.x;

	Debug::out("%f\n",cap.vel.y);
}

Data CaptainInjured::Exit(Captain& cap, State toState)
{
	Data data;
	data.Add<float>("waterLevel", waterLevel);
	return std::move(data);
}

void CaptainInjured::OnKeyUp(Captain& cap, BYTE keyCode)
{
}

void CaptainInjured::OnKeyDown(Captain& cap, BYTE keyCode)
{
}

void CaptainInjured::Update(Captain& cap, float dt, const std::vector<GameObject*>& coObjects)
{
	//Todo: Health <0 --->Die
	if (cap.health.Get()<=0)
	{
		cap.SetState(State::Captain_Dead);
		return;
	}
	//Collision objects left
	HandleCollisions(cap, dt, coObjects);
}

void CaptainInjured::HandleCollisions(Captain& cap, float dt, const std::vector<GameObject*>& coObjects)
{
	auto coEvents = CollisionDetector::CalcPotentialCollisions(cap, coObjects, dt);
	if (coEvents.size() == 0) { HandleNoCollisions(cap, dt); return; }

	float min_tx, min_ty, nx, ny;
	CollisionDetector::FilterCollisionEvents(coEvents, min_tx, min_ty, nx, ny);

	if (coEvents.size() == 0) return;

	cap.pos.x += min_tx * cap.vel.x * dt;
	cap.pos.y += min_ty * cap.vel.y * dt;

	if (cap.nx > 0)
	{
		if (posxWhenGotInjure - cap.pos.x >= INJURE_DISTANCE)
		{
			cap.SetState(State::Captain_Standing);
			return;
		}
	}
	else if (cap.nx < 0)
	{
		if (cap.pos.x - posxWhenGotInjure >= INJURE_DISTANCE)
		{
			cap.SetState(State::Captain_Standing);
			return;
		}
	}
	for (auto& e : coEvents)
	{
		if (auto spawner = dynamic_cast<Spawner*>(e.pCoObj))
		{
			//spawner->OnCollideWithCap();
			cap.CollideWithPassableObjects(dt, e); // go the remaining distance
		}
		else if (auto ambush = dynamic_cast<AmbushTrigger*>(e.pCoObj))
		{
			//ambush->OnCollideWithCap();
			cap.CollideWithPassableObjects(dt, e);
		}
		else if (auto item = dynamic_cast<Item*>(e.pCoObj))
		{
			//item->BeingCollected();
			cap.CollideWithPassableObjects(dt, e);
		}
		else if (auto enemy = dynamic_cast<Enemy*>(e.pCoObj))
		{
			cap.CollideWithPassableObjects(dt, e);
			//if (cap.isFlashing) { // undamagable
			//	cap.CollideWithPassableObjects(dt, e);
			//}
			//else {
			//	cap.SetState(State::Captain_Injured);
			//	cap.health.Subtract(1);
			//	enemy->TakeDamage(1);
			//}
		}
		else if (auto block = dynamic_cast<Block*>(e.pCoObj)) {

			switch (block->GetType())
			{
			case ClassId::Water:
				return;
				if (e.ny < 0) cap.SetState(State::Captain_FallToWater);
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
				if (!cap.isFlashing)
				{
					cap.health.Subtract(1);
					cap.SetState(State::Captain_Injured);
				}
				break;

			case ClassId::PassableLedge:
			case ClassId::RigidBlock:
				break;

			default:
				AssertUnreachable();
			}
		}
		else if (dynamic_cast<Capsule*>(e.pCoObj)) {
			cap.CollideWithPassableObjects(dt, e);
		}
		else if (dynamic_cast<Bullet*>(e.pCoObj)) {
			if (!cap.isFlashing)
			{
				cap.SetState(State::Captain_Injured);
				cap.health.Subtract(1);
			}
		}
	}


}

