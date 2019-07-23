#include "pch.h"
#include "CaptainJumping.h"


void CaptainJumping::Enter(Captain& cap, State fromState, Data&& data)
{
	assert(fromState == State::Captain_Climbing || fromState == State::Captain_CoverLow 
		|| fromState == State::Captain_CoverTop || fromState == State::Captain_Sitting 
		|| fromState == State::Captain_Standing || fromState == State::Captain_Walking
		|| fromState == State::Captain_InWater  || fromState == State::Captain_Swimming
	    || fromState == State::Captain_Kicking);
	if (fromState != State::Captain_Kicking) {
		 isJumpReleased = false;
		 JumpHeightRealCounter = 0;
		 JumpHeightNeedCounter = MIN_JUMP_HEIGHT;
	}
	switch (fromState)
	{
		case State::Captain_Climbing:
			break;
		case State::Captain_InWater:
			break;
		case State::Captain_CoverLow:
			break;
		case State::Captain_Swimming:
			cap.vel.y = -JUMP_SPEED_VER;
			break;
		case State::Captain_CoverTop:
			break;
		case State::Captain_Sitting:
			break;
		case State::Captain_Standing:
			break;
		case State::Captain_Walking:
			if (cap.lastKeyDown == KeyControls::Down) {
				cap.vel.y = JUMP_SPEED_VER;
			} else {
				cap.vel.y = -JUMP_SPEED_VER;
			}
			break;
		default:
			AssertUnreachable();
			break;
	}
}

Data CaptainJumping::Exit(Captain& cap, State toState)
{
	Data data;
	switch (toState)
	{
		case State::Captain_Kicking:
			data.Add(IS_JUMP_RELEASED, isJumpReleased);
			data.Add(JUMP_HEIGHT_RealCounter, JumpHeightRealCounter);
			data.Add(JUMP_HEIGHT_NeedCounter, JumpHeightNeedCounter);
			break;
	}
	return std::move(data);
}

void CaptainJumping::OnKeyUp(Captain& cap, BYTE keyCode)
{
	if (keyCode == setting.Get(KeyControls::Jump))
		isJumpReleased = true;
}

void CaptainJumping::OnKeyDown(Captain& cap, BYTE keyCode)
{
	if (keyCode == setting.Get(KeyControls::Attack))
	{
		cap.SetState(State::Captain_Kicking);
	}
	else {
		if (keyCode == setting.Get(KeyControls::Left)) {
			cap.nx = -1;
		}
		else if (keyCode == setting.Get(KeyControls::Right)) {
			cap.nx = 1;
		}
	}
		
}

void CaptainJumping::Update(Captain& cap, float dt, const std::vector<GameObject*>& coObjects)
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
	if (JumpHeightNeedCounter < MAX_JUMP_HEIGHT) {
		if (!isJumpReleased) {
			JumpHeightNeedCounter += JUMP_SPEED * dt;
			cap.vel.y = -JUMP_SPEED;
			JumpHeightRealCounter += JUMP_SPEED *dt;
		}
		else {
			if (JumpHeightRealCounter < JumpHeightNeedCounter)
			{
				cap.vel.y = -JUMP_SPEED;
				JumpHeightRealCounter += JUMP_SPEED*dt;
			}
			else
			{
				cap.SetState(State::Captain_Falling);
			}
		}
	}
	else {
		if (JumpHeightRealCounter < JumpHeightNeedCounter)
		{
			cap.vel.y = -JUMP_SPEED;
			JumpHeightRealCounter += JUMP_SPEED*dt;
		}
		else
		{
			cap.SetState(State::Captain_Spinning);
		}
	}

	//if (cap.animations.at(cap.curState).IsDoneCycle())
	//{
	//	static int count = 0;
	//	if (isJumpReleased) {
	//		// falling
	//		cap.vel.y = JUMP_SPEED_VER;
	//		isJumpReleased = false;
	//	}
	//	else // still holding jump
	//	{
	//		if (canHigher) {
	//			// automatic do one more cycle
	//			canHigher = false;
	//		}
	//		else // jump too high
	//		{
	//			cap.SetState(State::Captain_Spinning);
	//		}
	//	}

	//}
	//Debug::Out(cap.vel.y, 0.0001 * signed(cap.vel.y) * dt);
	//cap.vel.y += 0.0003 * signed(cap.vel.y) * dt;
	//Debug::Out(cap.vel.y, 0.0001 * signed(cap.vel.y) * dt);
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
				ambush->Active();
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
					if (!cap.isFlashing)
					{
						cap.health.Subtract(1);
						cap.SetState(State::Captain_Injured);
					}
					break;

				case ClassId::PassableLedge:
					if (e.ny < 0) {
						cap.SetState(State::Captain_Sitting);
						Sounds::PlayAt(SoundId::Grounding);
					} else {
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