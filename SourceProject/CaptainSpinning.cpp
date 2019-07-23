#include "pch.h"
#include "CaptainSpinning.h"
#include "BulletEnemyRocket.h"



void CaptainSpinning::Enter(Captain& cap, State fromState, Data&& data)
{
	assert(fromState == State::Captain_Jumping || fromState == State::Captain_Kicking);
	cap.vel.y = -SPIN_SPEED_VER;
}

Data CaptainSpinning::Exit(Captain& cap, State toState)
{
	Data data;
	timeUp = 0;
	timeDown = 0;
	// i dont thing we need to pass data in this case
	return data;
}

void CaptainSpinning::OnKeyUp(Captain& cap, BYTE keyCode)
{

}

void CaptainSpinning::OnKeyDown(Captain& cap, BYTE keyCode)
{
	if (keyCode == setting.Get(KeyControls::Attack))
	{
		cap.SetState(State::Captain_Kicking);
	}
}

void CaptainSpinning::Update(Captain& cap, float dt, const std::vector<GameObject*>& coObjects)
{
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
			cap.SetState(State::Captain_CoverLow);
	}

	if (cap.animations.at(cap.curState).IsDoneCycle())
	{
		if (timeUp < TIME_KEEP_SPIN) {
			timeUp += GameTimer::Dt();
			cap.vel.y = -SPIN_SPEED_HOR;
		}
		else
		{
			if (timeDown < TIME_KEEP_SPIN)
			{
				timeDown += GameTimer::Dt();
				cap.vel.y = SPIN_SPEED_HOR;
			}
			else
			{
				//cap.SetState(State::Captain_Jumping);
				cap.SetState(State::Captain_Falling);
			}
		}
	}

	HandleCollisions(cap, dt, coObjects);
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
			spawner->OnCollideWithCap();
			cap.CollideWithPassableObjects(dt, e); // go the remaining distance
		}
		else if (auto bullet = dynamic_cast<Bullet*>(e.pCoObj))
		{
			if (!cap.isFlashing)
			{
				if (auto bullet = dynamic_cast<BulletEnemyRocket*>(e.pCoObj));
				cap.health.Subtract(bullet->GetDamage());
				cap.SetState(State::Captain_Injured);
				//TODO: case BulletEnemyFlying
			}
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

			case ClassId::DamageBlock: //case that DamageBlock is at the roof
				if (!cap.isFlashing)
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
	}
}

