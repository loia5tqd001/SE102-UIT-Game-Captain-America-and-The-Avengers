#include "pch.h"
#include "CaptainJumping.h"
#include"BulletDynamite.h"
#include"ElectricBat.h"

void CaptainJumping::Enter(Captain& cap, State fromState, Data&& data)
{
	setAnotherState = false;
	if (fromState != State::Captain_FallToWater) { JumpSpeed = JUMP_SPEED_VER_MAX; acceleration = GRAVITY; }

	if (fromState == State::Captain_InWater || fromState == State::Captain_Swimming||fromState==State::Captain_FallToWater) {
		isFromWater = true;
		maxJumpHeight = MAX_JUMP_HEIGHT * 0.75f;
	}
	else {
		isFromWater = false;
		maxJumpHeight = MAX_JUMP_HEIGHT;
	}

	cap.vel.x = 0.0f; // avoid wind blowing phenomenon when jump from water
	cap.vel.y = 0.0f;
	if (fromState == State::Captain_Swimming ||
		fromState == State::Captain_InWater ||
		fromState == State::Captain_FallToWater)
	{
		Sounds::PlayAt(SoundId::Water);
	}
	if (fromState == State::Captain_Kicking)
	{
		JumpHeightRealCounter = data.Get<float>(JUMP_HEIGHT_RealCounter);
		JumpHeightNeedCounter = data.Get<float>(JUMP_HEIGHT_NeedCounter);
		isJumpReleased = data.Get<bool>(IS_JUMP_RELEASED);
	}
	else {
		isKicked = false;
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
		}
		else {
			cap.vel.y = -JUMP_SPEED_VER;
		}
		break;
	default:
		break;
	}
}

Data CaptainJumping::Exit(Captain& cap, State toState)
{
	data.Add(IS_JUMP_RELEASED, isJumpReleased);
	data.Add(JUMP_HEIGHT_RealCounter, JumpHeightRealCounter);
	data.Add(JUMP_HEIGHT_NeedCounter, JumpHeightNeedCounter);
	data.Add(IS_KICKED, isKicked);
	data.Add(SPIN_TIME_DOWN, 0.0f);
	data.Add(SPIN_TIME_UP, 0.0f);
	data.Add(IS_JUMP_FROM_WATER, isFromWater);
	return std::move(data);
}

void CaptainJumping::OnKeyUp(Captain& cap, BYTE keyCode)
{
	if (keyCode == setting.Get(KeyControls::Jump))
		isJumpReleased = true;
	if (keyCode == setting.Get(KeyControls::Left))
		cap.vel.x = 0;
	if (keyCode == setting.Get(KeyControls::Right))
		cap.vel.x = 0;
}

void CaptainJumping::OnKeyDown(Captain& cap, BYTE keyCode)
{
	if (keyCode == setting.Get(KeyControls::Attack))
	{
		if (!isKicked) {
			isKicked = true;
			cap.SetState(State::Captain_Kicking);
			return;
		}
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
	HandleCollisions(cap, dt, coObjects);

	if (setAnotherState)
		return;

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
	if (JumpHeightNeedCounter < maxJumpHeight) {
		if (!isJumpReleased) {
			JumpHeightNeedCounter += (JumpSpeed - 30) * dt;
			cap.vel.y = -JumpSpeed;
			JumpHeightRealCounter += JumpSpeed * dt;
		}
		else {
			if (JumpHeightRealCounter < JumpHeightNeedCounter*8.5f / 10.0f)
			{
				cap.vel.y = -JumpSpeed;
				JumpHeightRealCounter += JumpSpeed * dt;
			}
			else if (JumpHeightRealCounter < JumpHeightNeedCounter * 14 / 15)
			{
				cap.vel.y = -JumpSpeed / 2;
				JumpHeightRealCounter += JumpSpeed / 2 * dt;
			}
			else if (JumpHeightRealCounter < JumpHeightNeedCounter * 19 / 20)
			{
				cap.vel.y = -JumpSpeed / 3;
				JumpHeightRealCounter += JumpSpeed / 3 * dt;
			}
			else if (JumpHeightRealCounter < JumpHeightNeedCounter) {
				cap.vel.y = -JumpSpeed / 4;
				JumpHeightRealCounter += JumpSpeed / 4 * dt;
			}
			else
			{
				cap.SetState(State::Captain_Falling);
				return;
			}
		}
	}
	else {
		if (JumpHeightRealCounter < JumpHeightNeedCounter)
		{
			cap.vel.y = -JumpSpeed;
			JumpHeightRealCounter += JumpSpeed * dt;
		}
		else
		{
			cap.SetState(State::Captain_Spinning);
			return;
		}
	}
	acceleration += GRAVITY * dt;
	if (JumpSpeed >= JUMP_SPEED_VER_MIN)
	{
		//JumpSpeed -= acceleration * dt;
	}
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
			spawner->OnCollideWithCap(&cap);
			cap.CollideWithPassableObjects(dt, e); // go the remaining distance
		}
		else if (auto ambush = dynamic_cast<AmbushTrigger*>(e.pCoObj))
		{
			ambush->OnCollideWithCap(&cap);
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
			else
			{
				setAnotherState = true;
				enemy->TakeDamage(1);
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
					if (bat->GetState()==State::ElectricBat_FlyAttack)
					{
						cap.SetState(State::CaptainElectricShock);
						return;
					}
				}
				cap.SetState(State::Captain_Injured);
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
				cap.CollideWithPassableObjects(dt, e);
				break;

			case ClassId::DamageBlock:
			case ClassId::PassableLedge:
				cap.CollideWithPassableObjects(dt, e);
				break;

			case ClassId::RigidBlock:
				if (e.ny < 0) {
					cap.SetState(State::Captain_Sitting);
					Sounds::PlayAt(SoundId::Grounding);
				}
				else if (e.ny > 0) {
					cap.SetState(State::Captain_Falling);
					break;
				}
				break;

			default:
				AssertUnreachable();
			}
		}
		else if (dynamic_cast<Capsule*>(e.pCoObj)) {
			cap.CollideWithPassableObjects(dt, e);
		}
		else if (auto bullet = dynamic_cast<Bullet*>(e.pCoObj)) {
			if (!cap.isFlashing)
			{
				cap.health.Subtract(1);
				bullet->HitCaptain();
				cap.SetState(State::Captain_Injured);
				setAnotherState = true;
				return;
			}
			else if (dynamic_cast<BulletDynamite*>(e.pCoObj))
			{
				cap.CollideWithPassableObjects(dt, e);
			}
		}
		else if (auto movingLedgeUpdater = dynamic_cast<MovingLedge*>(e.pCoObj))
		{
			if (e.ny > 0)
			{
				cap.CollideWithPassableObjects(dt, e);
			}
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