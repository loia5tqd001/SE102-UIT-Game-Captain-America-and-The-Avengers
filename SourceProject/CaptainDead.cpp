#include "pch.h"
#include "CaptainDead.h"



void CaptainDead::Enter(Captain& cap, State fromState, Data&& data)
{
	initialized = false;
	Sounds::StopAll();
	Sounds::PlayAt(SoundId::Death);
	cap.isFlashing = true;
	cap.vel.x = 0.0f;
	cap.vel.y = 50.0f;
}

Data CaptainDead::Exit(Captain& cap, State toState)
{
	return Data{};
}

void CaptainDead::OnKeyUp(Captain& cap, BYTE keyCode)
{
}

void CaptainDead::OnKeyDown(Captain& cap, BYTE keyCode)
{
}

void CaptainDead::Update(Captain& cap, float dt, const std::vector<GameObject*>& coObjects)
{
	if (!initialized)
	{
		cap.isFlashing = true;
		cap.vel.x = 0.0f;
		cap.vel.y = 100.0f;
		initialized = true;
	}

	HandleCollisions(cap, dt, coObjects);

	if (cap.animations.at(cap.curState).IsDoneCycle())
	{
		sceneManager.GetCurScene().DoTransitionScene();
		cap.health.Set(12);
		cap.SetState(State::Captain_Standing);
	}
}

void CaptainDead::HandleCollisions(Captain& cap, float dt, const std::vector<GameObject*>& coObjects)
{
	auto coEvents = CollisionDetector::CalcPotentialCollisions(cap, coObjects, dt);
	if (coEvents.size() == 0)
	{
		cap.pos.x += cap.vel.x*dt;
		cap.pos.y += cap.vel.y*dt;
		return;
	}

	float min_tx, min_ty, nx, ny;
	CollisionDetector::FilterCollisionEvents(coEvents, min_tx, min_ty, nx, ny);

	if (coEvents.size() == 0) return;

	cap.pos.x += min_tx * cap.vel.x * dt;
	cap.pos.y += min_ty * cap.vel.y * dt;


	for (auto& e : coEvents)
	{
		if (auto movingLedgeUpdater = dynamic_cast<MovingLedgeUpdater*>(e.pCoObj))
		{
			cap.CollideWithPassableObjects(dt, e);
		}
		else if (auto block = dynamic_cast<Block*>(e.pCoObj)) {

			switch (block->GetType())
			{
			case ClassId::PassableLedge:
				return;
			case ClassId::Water:
				//Impossible
				break;
			case ClassId::ClimbableBar:
				cap.CollideWithPassableObjects(dt, e);
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
			return;
		}
		else if (auto movingLedge = dynamic_cast<MovingLedge*>(e.pCoObj))
		{
			//Dead on movingLedge???
			return;
		}
		else if (auto trap = dynamic_cast<ElectricTrap*>(e.pCoObj))
		{
			cap.CollideWithPassableObjects(dt, e);
		}
	}
}
