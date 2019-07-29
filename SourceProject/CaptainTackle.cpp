#include "pch.h"
#include "CaptainTackle.h"
#include "BulletEnemyGun.h"



void CaptainTackle::Enter(Captain& cap, State fromState, Data&& data)
{
	assert(fromState == State::Captain_Sitting || fromState == State::Captain_Injured);
	cap.vel.x = cap.nx * TACKLE_SPEED;
	cap.vel.y = JUMP_SPEED_VER;
	isStillOnGround = true;
	Sounds::PlayAt(SoundId::Tackle);
}

Data CaptainTackle::Exit(Captain& cap, State toState)
{
	return Data{};
}

void CaptainTackle::OnKeyUp(Captain& cap, BYTE keyCode)
{

}

void CaptainTackle::OnKeyDown(Captain& cap, BYTE keyCode)
{
}

void CaptainTackle::Update(Captain& cap, float dt, const std::vector<GameObject*>& coObjects)
{
	if (!isStillOnGround) {
		cap.SetState(State::Captain_Falling);
		return;
		// TODO: Set falling
	}
	if (cap.animations.at(cap.curState).IsDoneCycle())
	{
		cap.SetState(State::Captain_Standing);
		return;
	}
	else
	{
		auto kControlDir = cap.nx > 0 ? KeyControls::Right : KeyControls::Left;
		if (!wnd.IsKeyPressed(setting.Get(kControlDir)))
		{
			std::chrono::duration<float> duration = std::chrono::steady_clock::now() - cap.timeLastKeyUp;
			if (duration.count() > 0.1f) {
				cap.SetState(State::Captain_Standing);
			}
		}
	}

	HandleCollisions(cap, dt, coObjects); 
}

void CaptainTackle::HandleCollisions(Captain& cap, float dt, const std::vector<GameObject*>& coObjects)
{
	isStillOnGround = false;

	auto coEvents = CollisionDetector::CalcPotentialCollisions(cap, coObjects, dt);
	if (coEvents.size() == 0)
	{
		cap.pos.x += cap.vel.x * dt;
		cap.pos.y += cap.vel.y * dt;
		isStillOnGround = false;
		return;
	}

	float min_tx, min_ty, nx, ny;
	CollisionDetector::FilterCollisionEvents(coEvents, min_tx, min_ty, nx, ny);

	if (coEvents.size() == 0) return;

	cap.pos.x += min_tx * cap.vel.x * dt;
	cap.pos.y += min_ty * cap.vel.y * dt;

	for (auto& e : coEvents)
	{
		if (auto block = dynamic_cast<Block*>(e.pCoObj)) {

			switch (block->GetType())
			{
				case ClassId::RigidBlock:
					if (e.ny < 0) isStillOnGround = true;
					if (e.nx != 0) cap.SetState(State::Captain_Standing);
					break;
				case ClassId::PassableLedge:
					if (e.ny < 0) isStillOnGround = true;
					if (e.ny > 0) AssertUnreachable();
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
					break;

				case ClassId::ClimbableBar:
				case ClassId::Water:
				default:
					AssertUnreachable();
			}
		}
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
		else if (dynamic_cast<MovingLedge*>(e.pCoObj) || dynamic_cast<BreakableLedge*>(e.pCoObj))
		{
			isStillOnGround = true;
		}
		else if (dynamic_cast<Capsule*>(e.pCoObj)) {
			cap.CollideWithPassableObjects(dt, e);
		}
		else if (auto item = dynamic_cast<Item*>(e.pCoObj))
		{
			item->BeingCollected();
			cap.CollideWithPassableObjects(dt, e);
		}
		else if (auto enemy = dynamic_cast<Enemy*>(e.pCoObj))
		{
			enemy->TakeDamage(3);
			cap.CollideWithPassableObjects(dt, e);
		}
		else if (auto bullet = dynamic_cast<Bullet*>(e.pCoObj)) {
			if (!cap.isFlashing)
			{
				cap.health.Subtract(bullet->GetDamage());
				cap.SetState(State::Captain_Injured);
			}
			else {
				cap.CollideWithPassableObjects(dt, e);
			}
		}
	}
}
