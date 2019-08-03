#include "pch.h"
#include "CaptainSpinning.h"
#include "BulletEnemyRocket.h"
#include"BulletDynamite.h"


void CaptainSpinning::Enter(Captain& cap, State fromState, Data&& data)
{
	SetAnotherState = false;
	// bug from falling change to
	//assert(fromState == State::Captain_Jumping || fromState == State::Captain_Kicking || fromState == State::Captain_Injured);
	cap.vel.y = -SPIN_SPEED_VER;
	beginnx = cap.nx;

	isKicked = data.Get<bool>(IS_KICKED);
	if (data.Get<bool>(IS_JUMP_FROM_WATER))
		cap.SetState(State::Captain_Falling);
	if (isKicked)
	{
		if (fromState == State::Captain_Kicking || fromState == State::Captain_Jumping) {
			timeUp = data.Get<float>(SPIN_TIME_UP);
			timeDown = data.Get<float>(SPIN_TIME_DOWN);
		}
	}
	else
	{
		timeUp = 0;
		timeDown = 0;
	}
	if (!data.Get<bool>(IS_JUMP_FROM_WATER))
		Sounds::PlayAt(SoundId::Spinning);
}

Data CaptainSpinning::Exit(Captain& cap, State toState)
{
	cap.nx = beginnx;
	data.Add(IS_KICKED, isKicked);
	data.Add(SPIN_TIME_DOWN, timeDown);
	data.Add(SPIN_TIME_UP, timeUp);
	data.Add(IS_JUMP_FROM_WATER, false);
	return std::move(data);
}

void CaptainSpinning::OnKeyUp(Captain& cap, BYTE keyCode)
{
	if (keyCode == setting.Get(KeyControls::Left))
		cap.vel.x = 0;
	if (keyCode == setting.Get(KeyControls::Right))
		cap.vel.x = 0;
}

void CaptainSpinning::OnKeyDown(Captain& cap, BYTE keyCode)
{
	if (!isKicked) {
		if (keyCode == setting.Get(KeyControls::Attack))
		{
			isKicked = true;
			cap.SetState(State::Captain_Kicking);
			return;
		}
	}
}

void CaptainSpinning::Update(Captain& cap, float dt, const std::vector<GameObject*>& coObjects)
{
	if (counterTimeFlip >= 0.15f)
	{
		counterTimeFlip = 0;
	}
	else counterTimeFlip += GameTimer::Dt();
	cap.animations.at(cap.curState).Update(dt);
	HandleCollisions(cap, dt, coObjects);
	if (SetAnotherState)
	{
		return;
	}
	if (wnd.IsKeyPressed(setting.Get(KeyControls::Left)))
	{
		cap.vel.x = -SPIN_SPEED_HOR;
	}
	if (wnd.IsKeyPressed(setting.Get(KeyControls::Right)))
	{
		cap.vel.x = SPIN_SPEED_HOR;
	}
	if (wnd.IsKeyPressed(setting.Get(KeyControls::Down)))
	{
		if (timeUp >= TIME_KEEP_SPIN && timeDown >= TIME_KEEP_SPIN)
		{
			cap.SetState(State::Captain_CoverLow);
			return;
		}
	}

	if (timeUp < TIME_KEEP_SPIN) {
		timeUp += GameTimer::Dt();
		cap.vel.y = -SPIN_SPEED_VER;
	}
	else
	{
		if (timeDown < TIME_KEEP_SPIN)
		{
			timeDown += GameTimer::Dt();
			cap.vel.y = SPIN_SPEED_VER;
		}
		else
		{
			isKicked = false;
			cap.SetState(State::Captain_Falling);
			return;
		}
	}
}

void CaptainSpinning::HandleCollisions(Captain& cap, float dt, const std::vector<GameObject*>& coObjects)
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
		else if (auto bullet = dynamic_cast<Bullet*>(e.pCoObj))
		{
			if (!cap.isFlashing)
			{
				if (auto bullet = dynamic_cast<BulletEnemyRocket*>(e.pCoObj));
				cap.health.Subtract(bullet->GetDamage());
				bullet->HitCaptain();
				cap.SetState(State::Captain_Injured);
				SetAnotherState = true;
				//TODO: case BulletEnemyFlying
			}
			else if (dynamic_cast<BulletDynamite*>(e.pCoObj))
			{
				cap.CollideWithPassableObjects(dt, e);
			}

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
				cap.health.Subtract(1);
				if (auto mini = dynamic_cast<DynamiteNapalm*>(e.pCoObj))
				{
					if (mini->CanCauseElectricShock())
					{
						cap.SetState(State::CaptainElectricShock);
						return;
					}
				}
				cap.SetState(State::Captain_Injured);
				SetAnotherState = true;
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

			case ClassId::ClimbableBar:
			case ClassId::Switch:
			case ClassId::Door:
				cap.CollideWithPassableObjects(dt, e);
				break;


			case ClassId::DamageBlock: //case that DamageBlock is at the roof
				break;

			case ClassId::PassableLedge:
				// can go from floor to the ledge
				//cap.CollideWithPassableObjects(dt, e);
			case ClassId::RigidBlock:
				break;

			default:
				AssertUnreachable();
			}
		}
		else if (dynamic_cast<Capsule*>(e.pCoObj)) {
			cap.CollideWithPassableObjects(dt, e);
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
	}
}

