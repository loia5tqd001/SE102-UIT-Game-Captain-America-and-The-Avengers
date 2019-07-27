#include "pch.h"
#include "CaptainThrowing.h"
#include "BulletEnemyRocket.h"



void CaptainThrowing::Enter(Captain& cap, State fromState, Data&& data)
{
	assert(fromState == State::Captain_Jumping || 
		   fromState == State::Captain_CoverTop || 
		   fromState == State::Captain_Walking ||
		   fromState == State::Captain_Standing);
	assert(cap.shieldOn);
	Sounds::PlayAt(SoundId::ShieldThrow);
	cap.vel.y = 0;
	cap.vel.x = 0;
	cap.shield->ThrowAway();
}

Data CaptainThrowing::Exit(Captain& cap, State toState)
{
	return Data{};
}

void CaptainThrowing::OnKeyUp(Captain& cap, BYTE keyCode)
{
}

void CaptainThrowing::OnKeyDown(Captain& cap, BYTE keyCode)
{
	if (keyCode == setting.Get(KeyControls::Jump))
	{
		if (!cap.animations.at(cap.curState).IsDoneCycle())
		{
			//cap.shield->UpdateByCapState(cap.GetState(), cap.GetPos());
		}
		cap.SetState(State::Captain_Jumping);
		return;
	}
}

void CaptainThrowing::Update(Captain& cap, float dt, const std::vector<GameObject*>& coObjects)
{
	HandleCollisions(cap, dt, coObjects);
	if (cap.animations.at(State::Captain_Throwing).IsDoneCycle()) {
		if (wnd.IsKeyPressed(setting.Get(KeyControls::Down)))
		{
			cap.SetState(State::Captain_Sitting);
			return;
		}
		if (wnd.IsKeyPressed(setting.Get(KeyControls::Up)))
		{
			cap.SetState(State::Captain_CoverTop);
			return;
		}
		cap.SetState(State::Captain_Standing);
	}
}

void CaptainThrowing::HandleCollisions(Captain& cap, float dt, const std::vector<GameObject*>& coObjects)
{
	auto coEvents = CollisionDetector::CalcPotentialCollisions(cap, coObjects, dt);

	for (auto& e : coEvents)
	{
		if (auto bullet = dynamic_cast<Bullet*>(e.pCoObj))
		{
			if (!cap.isFlashing)
			{
				if (auto bullet = dynamic_cast<BulletEnemyRocket*>(e.pCoObj));
				cap.health.Subtract(bullet->GetDamage());
				bullet->HitCaptain();
				cap.SetState(State::Captain_Injured);
				//TODO: case BulletEnemyFlying
			}
		}
		else if (auto enemy = dynamic_cast<Enemy*>(e.pCoObj))
		{
			if (cap.isFlashing) { // undamagable
				cap.CollideWithPassableObjects(dt, e);
			}
			else {
				cap.health.Subtract(1);
				cap.SetState(State::Captain_Injured);
				enemy->TakeDamage(1);
			}
		}
		else if (auto block = dynamic_cast<Block*>(e.pCoObj))
		{
			if (block->GetType() == ClassId::DamageBlock)
			{
				cap.CollideWithPassableObjects(dt, e);
			}
		}
	}
}

