#include "pch.h"
#include "CaptainClimbing.h"



void CaptainClimbing::ProcessInput(Captain& cap)
{
	//Left-Right Arrow Press Handler
	if (KeyBuffer.size() == 0)
		return;
	int left = -1, right = -1;
	for (int i = 0;i < KeyBuffer.size();i++)
	{
		if (KeyBuffer.at(i) == setting.Get(KeyControls::Left))
			left = i;
		if (KeyBuffer.at(i) == setting.Get(KeyControls::Right))
			right = i;
	}

	if (left > right)
	{
		if (cap.nx == -1)
		{
			return;
		}
		else
		{
			cap.nx = -1;
			cap.pos.x = cap.pos.x + cap.GetBBox().GetWidth() / 2;
		}
	}

	if (left < right)
	{
		if (cap.nx == 1)
		{
			return;
		}
		else
		{
			cap.pos.x = cap.pos.x - cap.GetBBox().GetWidth() / 2;
			cap.nx = 1;
		}
	}

	KeyBuffer.clear();
}

void CaptainClimbing::Enter(Captain& cap, State fromState, Data&& data)
{
	//Todo: set position base on Jump Exit data
	cap.vel.x = 0.0f;
	cap.vel.y = 0.0f;
	switch (fromState)
	{
	case State::Captain_Falling:
	case State::Captain_Spinning:
		break;
	default:
		break;
		//AssertUnreachable(); BUG?
	}
}

Data CaptainClimbing::Exit(Captain& cap, State toState)
{
	return Data();
}

void CaptainClimbing::OnKeyUp(Captain& cap, BYTE keyCode)
{
}

void CaptainClimbing::OnKeyDown(Captain& cap, BYTE keyCode)
{
	if (KeyBuffer.size() >= bufferSize)
	{
		KeyBuffer.erase(KeyBuffer.begin());
	}
	KeyBuffer.push_back(keyCode);
	if (keyCode==setting.Get(KeyControls::Jump))
	{
		cap.SetState(State::Captain_Jumping);
		KeyBuffer.clear();
	}
	if (keyCode==setting.Get(KeyControls::Down))
	{
		cap.SetState(State::Captain_Falling);
		cap.pos.y += 1;
	}
}

void CaptainClimbing::Update(Captain& cap, float dt, const std::vector<GameObject*>& coObjects)
{
	ProcessInput(cap);
}

void CaptainClimbing::HandleCollisions(Captain& cap, float dt, const std::vector<GameObject*>& coObjects)
{
	auto coEvents = CollisionDetector::CalcPotentialCollisions(cap, coObjects, dt);
	if (coEvents.size() == 0)
		return;
	float min_tx, min_ty, nx, ny;
	CollisionDetector::FilterCollisionEvents(coEvents, min_tx, min_ty, nx, ny);

	if (coEvents.size() == 0) return;

	cap.pos.x += min_tx * cap.vel.x * dt;
	cap.pos.y += min_ty * cap.vel.y * dt;

	for (auto&e : coEvents)
	{
		if (auto spawner = dynamic_cast<Spawner*>(e.pCoObj))
		{
			spawner->OnCollideWithCap(&cap);
			cap.CollideWithPassableObjects(dt, e);
		}
		else if (auto ambushTrigger = dynamic_cast<AmbushTrigger*>(e.pCoObj))
		{
			ambushTrigger->OnCollideWithCap(&cap);
			cap.CollideWithPassableObjects(dt, e);
		}
		else if (auto item = dynamic_cast<Item*>(e.pCoObj))
		{
			item->BeingCollected();
			cap.CollideWithPassableObjects(dt, e);
		}
		else if (auto enemy = dynamic_cast<Enemy*>(e.pCoObj))
		{
			if (cap.isFlashing) //immortal
			{
				cap.CollideWithPassableObjects(dt, e);
			}
			else
			{
				cap.health.Subtract(1);
				cap.SetState(State::Captain_Injured);
				enemy->TakeDamage(1);
			}
		}
		else if (auto block = dynamic_cast<Block*>(e.pCoObj))
		{
			switch (block->GetType())
			{
			case ClassId::Water:
				break;
			case ClassId::NextMap:
				if (sceneManager.GetCurScene().canGoNextMap)
					sceneManager.GoNextScene();
				else
					cap.CollideWithPassableObjects(dt, e);
				break;



			case ClassId::Switch:
				break;
			case ClassId::Door:
				cap.CollideWithPassableObjects(dt, e);
				break;

			case ClassId::ClimbableBar:
				if (e.ny < 0)
					cap.SetState(State::Captain_Climbing);
				break;

			case ClassId::DamageBlock:
				cap.CollideWithPassableObjects(dt, e);
				break;

			case ClassId::PassableLedge:
				if (e.ny < 0) {
					cap.SetState(State::Captain_Sitting);
					Sounds::PlayAt(SoundId::Grounding);
				}
				else {
					cap.CollideWithPassableObjects(dt, e);
				}
				break;

			case ClassId::RigidBlock:
				if (e.ny > 0) {
					cap.SetState(State::Captain_Sitting);
					Sounds::PlayAt(SoundId::Grounding);
				}
				break;

			default:
				AssertUnreachable();
			}
		}
		else if (auto capsule = dynamic_cast<Capsule*>(e.pCoObj))
		{
			cap.CollideWithPassableObjects(dt, e);
		}
		else if (auto bullet = dynamic_cast<Bullet*>(e.pCoObj))
		{
			if (!cap.isFlashing)
			{
				cap.health.Subtract(bullet->GetDamage());
				bullet->HitCaptain();
				cap.SetState(State::Captain_Injured);
			}
		}
	}
}

