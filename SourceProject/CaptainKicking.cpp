#include "pch.h"
#include "CaptainKicking.h"
#include"BulletDynamite.h"
#include"ElectricBat.h"

void CaptainKicking::Enter(Captain& cap, State fromState, Data&& data)
{
	firstTimeUpdate = true;
	SetAnotherState = false;
	assert(fromState == State::Captain_Jumping || fromState == State::Captain_Spinning
		|| fromState == State::Captain_Falling || fromState == State::Captain_CoverLow);
	lastState = fromState;
	if (fromState == State::Captain_Jumping)
	{
		isJumpReleased = data.Get<bool>(IS_JUMP_RELEASED);
		JumpHeightRealCounter = data.Get<float>(JUMP_HEIGHT_RealCounter);
		JumpHeightNeedCounter = data.Get<float>(JUMP_HEIGHT_NeedCounter);
		isJumpFromWater = data.Get<bool>(IS_JUMP_FROM_WATER);
	}
	if (fromState == State::Captain_Spinning)
	{
		timeUp = data.Get<float>(SPIN_TIME_UP);
		timeDown = data.Get<float>(SPIN_TIME_DOWN);
	}
}

Data CaptainKicking::Exit(Captain& cap, State toState)
{
	Sounds::PlayAt(SoundId::Punch);
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
		data.Add(IS_JUMP_FROM_WATER, isJumpFromWater);
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
	HandleCollisions(cap, dt, coObjects);
	if (wnd.IsKeyPressed(setting.Get(KeyControls::Left)))
	{
		cap.vel.x = -MOVING_HOR;
	}
	if (wnd.IsKeyPressed(setting.Get(KeyControls::Right)))
	{
		cap.vel.x = MOVING_HOR;
	}

	if (SetAnotherState)
	{
		return;
	}

	if (lastState == State::Captain_Jumping) {
		if (JumpHeightNeedCounter < MAX_JUMP_HEIGHT) {
			if (!isJumpReleased) {
				JumpHeightNeedCounter += JUMP_SPEED_VER * dt;
				cap.vel.y = -JUMP_SPEED_VER;
				JumpHeightRealCounter += JUMP_SPEED_VER * dt;
			}
			else {
				if (JumpHeightRealCounter < JumpHeightNeedCounter)
				{
					cap.vel.y = -JUMP_SPEED_VER;
					JumpHeightRealCounter += JUMP_SPEED_VER * dt;
				}
				else
				{

				}
			}
		}
		else {
			if (JumpHeightRealCounter < JumpHeightNeedCounter)
			{
				cap.vel.y = -JUMP_SPEED_VER;
				JumpHeightRealCounter += JUMP_SPEED_VER * dt;
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
			return;
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
				{
					cap.SetState(State::Captain_Falling);
					return;
				}
			}
		}
		if (cap.animations.at(cap.curState).IsDoneCycle())
		{
			cap.SetState(State::Captain_Spinning);
			return;
		}
	}
	else if (lastState == State::Captain_Falling)
	{
		if (cap.animations.at(cap.curState).IsDoneCycle())
		{
			cap.SetState(State::Captain_Falling);
			return;
		}
	}
	else if (lastState == State::Captain_CoverLow)
	{
		if (cap.animations.at(cap.curState).IsDoneCycle())
		{
			cap.SetState(State::Captain_Falling);
			return;
		}
	}

}

void CaptainKicking::HandleCollisions(Captain& cap, float dt, const std::vector<GameObject*>& coObjects)
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
				if (auto mini = dynamic_cast<DynamiteNapalm*>(e.pCoObj))
				{
					if (mini->CanCauseElectricShock())
					{
						cap.SetState(State::CaptainElectricShock);
						return;
					}
				}
				else if (auto bat = dynamic_cast<ElectricBat*>(e.pCoObj))
				{
					if (bat->GetState() == State::ElectricBat_FlyAttack)
					{
						cap.SetState(State::CaptainElectricShock);
						return;
					}
				}

				cap.SetState(State::Captain_Injured);
				SetAnotherState = true;
			}
		}
		else if (auto dynamite = dynamic_cast<BulletDynamite*>(e.pCoObj))
		{
			dynamite->Trigger();
		}
		else if (auto block = dynamic_cast<Block*>(e.pCoObj))
		{
			switch (block->GetType())
			{
			case ClassId::Water:
				if (e.ny < 0)
				{
					SetAnotherState = true;
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
				cap.CollideWithPassableObjects(dt, e);
				break;

			case ClassId::DamageBlock:
				cap.CollideWithPassableObjects(dt, e);
				if (!cap.isFlashing)
				{
					cap.health.Subtract(1);
					cap.SetState(State::Captain_Injured);
					SetAnotherState = true;
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
				if (e.ny < 0) {
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
				bullet->HitCaptain();
				cap.health.Subtract(bullet->GetDamage());
				cap.SetState(State::Captain_Injured);
				SetAnotherState = true;
			}
			else if (dynamic_cast<BulletDynamite*>(e.pCoObj))
			{
				cap.CollideWithPassableObjects(dt, e);
			}

		}
		else if (dynamic_cast<MovingLedge*>(e.pCoObj))
		{
			if (e.ny > 0)
				cap.CollideWithPassableObjects(dt, e);
		}
		else if (auto movingLedgeUpdater = dynamic_cast<MovingLedgeUpdater*>(e.pCoObj))
		{
			cap.CollideWithPassableObjects(dt, e);
		}
		else if (auto trap = dynamic_cast<ElectricTrap*>(e.pCoObj))
		{
			cap.CollideWithPassableObjects(dt, e);
		}
		else if (auto door = dynamic_cast<Door*>(e.pCoObj))
		{
			cap.CollideWithPassableObjects(dt, e);
		}

	}
}


