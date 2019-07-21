#include "pch.h"
#include "CaptainSitPunching.h"
#include "BulletEnemyRocket.h"



void CaptainSitPunching::Enter(Captain& cap, State fromState, Data&& data)
{
	assert(cap.shieldOn);
	assert(fromState == State::Captain_Sitting);
	cap.vel.y = 0;
	cap.vel.x = 0;
}

Data CaptainSitPunching::Exit(Captain& cap, State toState)
{
	return Data{};
}

void CaptainSitPunching::OnKeyUp(Captain& cap, BYTE keyCode)
{
}

void CaptainSitPunching::OnKeyDown(Captain& cap, BYTE keyCode)
{
	assert(!cap.shieldOn);
}

void CaptainSitPunching::Update(Captain& cap, float dt, const std::vector<GameObject*>& coObjects)
{
	if (cap.animations.at(State::Captain_Throwing).IsDoneCycle()) {

		cap.SetState(State::Captain_Sitting); //move to Captain_Walking, Captain_Walking should handle nx, KeyControls press, up and down
	}
	HandleCollisions(cap, dt, coObjects);
}

void CaptainSitPunching::HandleCollisions(Captain& cap, float dt, const std::vector<GameObject*>& coObjects)
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
				cap.SetState(State::Captain_Injured);
				enemy->TakeDamage(1);
			}
		}	
	}
}

