#include "pch.h"
#include "CaptainWalking.h"
#include "ElectricBat.h"


void CaptainWalking::Enter(Captain& cap, State fromState, Data&& data)
{
	cap.vel.x = cap.nx * WALKING_SPEED;
}

Data CaptainWalking::Exit(Captain& cap, State toState)
{
	switch (toState)
	{
		case State::Captain_Sitting:
			data.Add(IS_TO_SIT_TACKLE, isToSittingTackle);
			break;

	}

	return std::move(data);
}

void CaptainWalking::OnKeyUp(Captain& cap, BYTE keyCode)
{

}

void CaptainWalking::OnKeyDown(Captain& cap, BYTE keyCode)
{
	if (keyCode == setting.Get(KeyControls::Jump))
	{
		cap.SetState(State::Captain_Jumping);
		return;
	}
	else if (keyCode == setting.Get(KeyControls::Attack))
	{
		if (cap.shieldOn) {
			cap.SetState(State::Captain_Throwing);
			return;
		}
		else {
			cap.SetState(State::Captain_Punching);
			return;
		}
	}


	auto kControlDir = cap.nx > 0 ? KeyControls::Right : KeyControls::Left;

	// if pressdown two time in a row in direction
	if (setting.Get(kControlDir) == keyCode &&
		cap.lastKeyDown == kControlDir && cap.lastKeyUp == kControlDir)
	{
		std::chrono::duration<float> duration = std::chrono::steady_clock::now() - cap.timeLastKeyDown;
		if (duration.count() < 0.14f)
		{
			isToSittingTackle = true;
			cap.SetState(State::Captain_Sitting);
			return;
		}
	}
}

void CaptainWalking::Update(Captain& cap, float dt, const std::vector<GameObject*>& coObjects)
{
	cap.vel.x = cap.nx * WALKING_SPEED;
	cap.vel.y = GRAVITY;
	cap.vel += onMovingLedgeSpeed * 0.5f;

	auto kControlDir = cap.nx > 0 ? KeyControls::Right : KeyControls::Left;
	if (!wnd.IsKeyPressed(setting.Get(kControlDir)))
	{
		cap.SetState(State::Captain_Standing);
		return;
	}
	HandleCollisions(cap, dt, coObjects);
	if (!isOnGround) {
		cap.SetState(State::Captain_Falling);
	}
}

void CaptainWalking::HandleCollisions(Captain& cap, float dt, const std::vector<GameObject*>& coObjects)
{
	onMovingLedgeSpeed = {};
	isOnGround = false;
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
				cap.health.Subtract(1);
				enemy->TakeDamage(1);
			}
		}
		else if (auto ledge = dynamic_cast<BreakableLedge*>(e.pCoObj))
		{
			isOnGround = true;
		}
		else if (auto ledge = dynamic_cast<MovingLedge*>(e.pCoObj))
		{
			if (e.ny < 0) isOnGround = true;
			onMovingLedgeSpeed = ledge->GetVelocity();
		}
		else if (auto block = dynamic_cast<Block*>(e.pCoObj)) {

			switch (block->GetType())
			{
				case ClassId::Water:
					// return;
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

				case ClassId::DamageBlock:
					cap.CollideWithPassableObjects(dt, e);
					isOnGround = true;
					break;

				case ClassId::PassableLedge:
				case ClassId::RigidBlock:
					if (e.ny < 0) isOnGround = true;
					break;

				default:
					//AssertUnreachable();
					break;
			}
		}
		else if (dynamic_cast<Capsule*>(e.pCoObj)) {
			cap.CollideWithPassableObjects(dt, e);
		}
		else if (auto bullet = dynamic_cast<Bullet*>(e.pCoObj)) {
			if (!cap.isFlashing)
			{
				bullet->HitCaptain();
				cap.health.Subtract(bullet->GetDamage());
				cap.SetState(State::Captain_Injured);
			}
		}
		else if (auto movingLedgeUpdater = dynamic_cast<MovingLedgeUpdater*>(e.pCoObj))
		{
			cap.CollideWithPassableObjects(dt, e);
		}
	}
}
