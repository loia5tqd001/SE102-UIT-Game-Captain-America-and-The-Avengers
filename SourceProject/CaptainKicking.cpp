#include "pch.h"
#include "CaptainKicking.h"



void CaptainKicking::Enter(Captain& cap, State fromState, Data&& data)
{
	assert(fromState == State::Captain_Jumping || fromState == State::Captain_Spinning || fromState == State::Captain_Falling);
	lastState = fromState;
	if (fromState == State::Captain_Jumping)
	{
		isJumpReleased = data.Get<bool>(IS_JUMP_RELEASED);
	    JumpHeightRealCounter = data.Get<float>(JUMP_HEIGHT_RealCounter);
		JumpHeightNeedCounter = data.Get<float>(JUMP_HEIGHT_NeedCounter);
	}
	if (fromState == State::Captain_Spinning)
	{
		timeUp = data.Get<float>(SPIN_TIME_UP);
		timeDown = data.Get<float>(SPIN_TIME_DOWN);
	}
	Sounds::PlayAt(SoundId::Punch);
}

Data CaptainKicking::Exit(Captain& cap, State toState)
{
	Data data;
	isKicked = true;
	switch (toState)
	{
	case State::Captain_Jumping:
		data.Add(JUMP_HEIGHT_RealCounter, JumpHeightRealCounter);
		data.Add(JUMP_HEIGHT_NeedCounter, JumpHeightNeedCounter);
		break;
	case State::Captain_Spinning:
		data.Add(SPIN_TIME_DOWN, timeDown);
		data.Add(SPIN_TIME_UP, timeUp);
		break;
	}
	data.Add(IS_JUMP_RELEASED, isJumpReleased);
	data.Add(IS_KICKED, isKicked);
	return std::move(data);
}

void CaptainKicking::OnKeyUp(Captain& cap, BYTE keyCode)
{
	if (keyCode == setting.Get(KeyControls::Jump))
		isJumpReleased = true;
	if (keyCode == setting.Get(KeyControls::Left))
		cap.vel.x = 0;
	if (keyCode == setting.Get(KeyControls::Right))
		cap.vel.x = 0;
}

void CaptainKicking::OnKeyDown(Captain& cap, BYTE keyCode)
{
}

void CaptainKicking::Update(Captain& cap, float dt, const std::vector<GameObject*>& coObjects)
{
	if (wnd.IsKeyPressed(setting.Get(KeyControls::Left)))
	{
		cap.vel.x = -MOVING_HOR;
	}
	if (wnd.IsKeyPressed(setting.Get(KeyControls::Right)))
	{
		cap.vel.x = MOVING_HOR;
	}
	if (lastState == State::Captain_Jumping) {
		if (JumpHeightNeedCounter < MAX_JUMP_HEIGHT) {
			if (!isJumpReleased) {
				JumpHeightNeedCounter += JUMP_SPEED * dt;
				cap.vel.y = -JUMP_SPEED;
				JumpHeightRealCounter += JUMP_SPEED * dt;
			}
			else {
				if (JumpHeightRealCounter < JumpHeightNeedCounter)
				{
					cap.vel.y = -JUMP_SPEED;
					JumpHeightRealCounter += JUMP_SPEED * dt;
				}
				else
				{

				}
			}
		}
		else {
			if (JumpHeightRealCounter < JumpHeightNeedCounter)
			{
				cap.vel.y = -JUMP_SPEED;
				JumpHeightRealCounter += JUMP_SPEED * dt;
			}
			else
			{
				timeUp += GameTimer::Dt();
				cap.vel.y = -SPIN_SPEED_HOR;
				if (cap.animations.at(cap.curState).IsDoneCycle())
				{
					cap.SetState(State::Captain_Spinning);
					return;
				}
			}
		}
		if (cap.animations.at(cap.curState).IsDoneCycle())
		{
			cap.SetState(State::Captain_Jumping);
		}
	}
	else if (lastState == State::Captain_Spinning)
	{
		if (timeUp < TIME_KEEP_SPIN) {
			timeUp += GameTimer::Dt();
			cap.vel.y = -SPIN_SPEED_HOR;
		}
		else
		{
			isKicked = false;
			if (timeDown < TIME_KEEP_SPIN)
			{
				timeDown += GameTimer::Dt();
				cap.vel.y = SPIN_SPEED_HOR;
			}
			else
			{
				if (cap.animations.at(cap.curState).IsDoneCycle())
					cap.SetState(State::Captain_Falling);
			}
		}
		if (cap.animations.at(cap.curState).IsDoneCycle())
		{
			cap.SetState(State::Captain_Spinning);
		}
	}
	else if (lastState == State::Captain_Falling)
	{
		if (cap.animations.at(cap.curState).IsDoneCycle())
			cap.SetState(State::Captain_Falling);
	}

	HandleCollisions(cap, dt, coObjects);
}

void CaptainKicking::HandleCollisions(Captain& cap, float dt, const std::vector<GameObject*>& coObjects)
{
	// collision with captain kick
	cap.pos.x += cap.vel.x*dt;
	cap.pos.y += cap.vel.y*dt;
	
	auto coEvents = CollisionDetector::CalcPotentialCollisions(cap, coObjects, dt);
	if (coEvents.size() == 0) { return; }

	float min_tx, min_ty, nx, ny;
	CollisionDetector::FilterCollisionEvents(coEvents, min_tx, min_ty, nx, ny);

	if (coEvents.size() == 0) return;


	for (auto&e : coEvents)
	{
		if (auto spawner = dynamic_cast<Spawner*>(e.pCoObj))
		{
			spawner->OnCollideWithCap(&cap);
			cap.CollideWithPassableObjects(dt, e);
		}
		else if (auto ambushTrigger = dynamic_cast<AmbushTrigger*>(e.pCoObj))
		{
			//ambushTrigger->Active();
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
				break;

			case ClassId::Switch:
				break;
				//Todo: Handle this
			case ClassId::Door:
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
				cap.SetState(State::Captain_Injured);
				cap.health.Subtract(1);
			}
		}
	}
}


