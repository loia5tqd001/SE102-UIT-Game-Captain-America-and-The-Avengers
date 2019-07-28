#include "pch.h"
#include "CaptainFalling.h"

void CaptainFalling::HandleNoCollisions(Captain & cap, float dt)
{
	cap.pos.x += cap.vel.x*dt;
	cap.pos.y += cap.vel.y*dt;
}

void CaptainFalling::Enter(Captain & cap, State fromState, Data && data)
{
	//Todo: Check if Cap is already on the ground or brick or something, switch to another state immediately
	cap.vel.y = FALL_SPEED_VER_MIN;

	if (fromState == State::Captain_Spinning || fromState == State::Captain_Jumping || fromState == State::Captain_Kicking) {
		isKicked = data.Get<bool>(IS_KICKED);
	}
}

Data CaptainFalling::Exit(Captain & cap, State toState)
{
	isKicked = false;
	data.Add(IS_KICKED, isKicked);
	return std::move(data);
}

void CaptainFalling::OnKeyUp(Captain & cap, BYTE keyCode)
{
	if (keyCode == setting.Get(KeyControls::Left))
	{
		if (wnd.IsKeyPressed(setting.Get(KeyControls::Right)))
		{
			cap.vel.x = FALL_SPEED_HOR;
			cap.nx = 1;
		}
		else
			cap.vel.x = 0;
		return;
	}

	if (keyCode == setting.Get(KeyControls::Right))
	{
		if (wnd.IsKeyPressed(setting.Get(KeyControls::Left)))
		{
			cap.vel.x = -FALL_SPEED_HOR;
			cap.nx = -1;
		}
		else
			cap.vel.x = 0;
		return;
	}
}

void CaptainFalling::OnKeyDown(Captain & cap, BYTE keyCode)
{
	if (keyCode == setting.Get(KeyControls::Left))
	{
		if (cap.vel.x >= 0)
		{
			cap.vel.x = -FALL_SPEED_HOR;
			cap.nx = -1;
		}
	}

	if (keyCode == setting.Get(KeyControls::Right))
	{
		if (cap.vel.x <= 0)
		{
			cap.vel.x = FALL_SPEED_HOR;
			cap.nx = 1;
		}
	}
	if (!isKicked)
	{
		if (keyCode == setting.Get(KeyControls::Attack))
		{
			isKicked = true;
			cap.SetState(State::Captain_Kicking);
		}
	}

}

void CaptainFalling::Update(Captain & cap, float dt, const std::vector<GameObject*>& coObjects)
{
	if (wnd.IsKeyPressed(setting.Get(KeyControls::Left)))
	{
		cap.vel.x = -MOVING_HOR;
		cap.nx = -1;
	}
	if (wnd.IsKeyPressed(setting.Get(KeyControls::Right)))
	{
		cap.vel.x = +MOVING_HOR;
		cap.nx = 1;
	}
	if (cap.vel.y < FALL_SPEED_VER_MAX)
	{
		cap.vel.y += GRAVITY * 2.5 * dt;// dt;
	}
	//Testing
	HandleCollisions(cap, dt, coObjects);
	//Endtesting
}

void CaptainFalling::HandleCollisions(Captain & cap, float dt, const std::vector<GameObject*>& coObjects)
{
	auto coEvents = CollisionDetector::CalcPotentialCollisions(cap, coObjects, dt);
	if (coEvents.size() == 0) { HandleNoCollisions(cap, dt); return; }

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
				if (e.ny < 0)
				{
					cap.SetState(State::Captain_FallToWater);
				}
				break;
			case ClassId::NextMap:
				if (sceneManager.GetCurScene().canGoNextMap)
					sceneManager.GoNextScene();
				else cap.CollideWithPassableObjects(dt, e);
				break;



			case ClassId::Switch: //Todo: Handle this
			case ClassId::Door:
				cap.CollideWithPassableObjects(dt, e);
				break;

			case ClassId::ClimbableBar:
				if (e.ny < 0)
				{
					data.Add("ClimbableBarRectF", block->GetBBox());
					cap.SetState(State::Captain_Climbing);
				}
				break;

			case ClassId::DamageBlock:
				cap.CollideWithPassableObjects(dt, e);
				break;

			case ClassId::PassableLedge:
				if (e.ny < 0) {
					cap.SetState(State::Captain_Sitting);
					//Sounds::PlayAt(SoundId::Grounding); // sound's bad now
				}
				else {
					//AssertUnreachable();
					cap.CollideWithPassableObjects(dt, e);
				}
				break;

			case ClassId::RigidBlock:
				if (e.nx == 0)
				{
					if (e.ny < 0) {
						cap.SetState(State::Captain_Sitting);
						//Sounds::PlayAt(SoundId::Grounding);
					}
					else {
						cap.vel.y = +FALL_SPEED_VER;
					}
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
