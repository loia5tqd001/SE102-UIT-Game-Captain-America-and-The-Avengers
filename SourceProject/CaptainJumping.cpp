#include "pch.h"
#include "CaptainJumping.h"


void CaptainJumping::Enter(Captain& cap, State fromState, Data&& data)
{
	switch (fromState)
	{
		case State::Captain_InWater:
		case State::Captain_Swimming:
			canHigher = false;
			break;
		default:
			canHigher = true;
			break;
	}
	isJumpReleased = false;
	cap.vel.y = -JUMP_SPEED_VER;
}

Data CaptainJumping::Exit(Captain& cap, State toState)
{
	Data data;
	switch (toState)
	{
		case State::Captain_Kicking:
			data.Add(CAN_JUMP_HIGHER, canHigher);
			data.Add(IS_JUMP_RELEASED, isJumpReleased);
			break;
	}
	return std::move(data);
}

void CaptainJumping::OnKeyUp(Captain& cap, BYTE keyCode)
{
	if (keyCode == setting.Get(KeyControls::Jump))
	{
		isJumpReleased = true;
	}
}

void CaptainJumping::OnKeyDown(Captain& cap, BYTE keyCode)
{
	if (keyCode == setting.Get(KeyControls::Attack))
	{
		cap.SetState(State::Captain_Kicking);
	}
}

void CaptainJumping::Update(Captain& cap, float dt, const std::vector<GameObject*>& coObjects)
{
	cap.vel.x = 0.0f;
	if (wnd.IsKeyPressed(setting.Get(KeyControls::Left)))
	{
		cap.vel.x -= JUMP_SPEED_HOR;
	}
	if (wnd.IsKeyPressed(setting.Get(KeyControls::Right)))
	{
		cap.vel.x += JUMP_SPEED_HOR;
	}

	if (cap.animations.at(cap.curState).IsDoneCycle())
	{
		if (canHigher) {
			if(isJumpReleased) {
				// falling
				cap.vel.y = JUMP_SPEED_VER;
			} 
			else {
				// auto do one more cycle animation
				canHigher = false;
			}
		}
		else // jump too high
		{
			if (isJumpReleased) {
				// falling
				cap.vel.y = JUMP_SPEED_VER;
			}
			else {
				cap.SetState(State::Captain_Spinning);
			}
		}
	}

	HandleCollisions(cap, dt, coObjects);
}


void CaptainJumping::HandleCollisions(Captain& cap, float dt, const std::vector<GameObject*>& coObjects)
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
		if (auto spawner = dynamic_cast<Spawner*>(e.pCoObj))
		{
			spawner->OnCollideWithCap();
			cap.CollideWithPassableObjects(dt, e); // go the remaining distance
		}
		else if (auto ambush = dynamic_cast<AmbushTrigger*>(e.pCoObj))
		{
			if (!ambush->IsActive())
				ambush->Active(coObjects);
			cap.CollideWithPassableObjects(dt, e);
		}
		else if (auto item = dynamic_cast<Item*>(e.pCoObj))
		{
			item->BeingCollected();
			cap.CollideWithPassableObjects(dt, e);
		}
		else if (auto enemy = dynamic_cast<Enemy*>(e.pCoObj))
		{
			if (cap.isFlashing) { // undamagable
				cap.CollideWithPassableObjects(dt, e);
			}
			else {
				cap.SetState(State::Captain_Injured);
				enemy->TakeDamage(1);
			}
		}
		else if (auto block = dynamic_cast<Block*>(e.pCoObj)) {

			switch (block->GetType())
			{
				case ClassId::Water:
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

				case ClassId::ClimbableBar:
					if (e.ny < 0)
						cap.SetState(State::Captain_Climbing);
					break;

				case ClassId::DamageBlock:
					cap.SetState(State::Captain_Injured);
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
			cap.SetState(State::Captain_Injured);
		}
	}
}