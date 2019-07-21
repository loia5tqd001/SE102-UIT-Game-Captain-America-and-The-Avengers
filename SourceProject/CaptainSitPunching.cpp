#include "pch.h"
#include "CaptainSitPunching.h"
#include "BulletEnemyRocket.h"



void CaptainSitPunching::Enter(Captain& cap, State fromState, Data&& data)
{
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

}

void CaptainSitPunching::Update(Captain& cap, float dt, const std::vector<GameObject*>& coObjects)
{
	if (cap.animations.at(State::Captain_Throwing).IsDoneCycle()) {

		cap.SetState(State::Captain_Sitting); //move to Captain_Sitting, Captain_Walking should handle nx, KeyControls press, up and down
	}
	HandleCollisions(cap, dt, coObjects);
}

void CaptainSitPunching::HandleCollisions(Captain& cap, float dt, const std::vector<GameObject*>& coObjects)
{
	auto coEvents = CollisionDetector::CalcPotentialCollisions(cap, coObjects, dt);

	float min_tx, min_ty, nx, ny;
	CollisionDetector::FilterCollisionEvents(coEvents, min_tx, min_ty, nx, ny);

	if (coEvents.size() == 0) return;

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
			if (cap.isFlashing) { // undamagable and can not damage enemy
				cap.CollideWithPassableObjects(dt, e);
			}
			else {
				if (nx * e.nx > 0)
				{
					enemy->TakeDamage(1);
				}
				else
				{
					cap.SetState(State::Captain_Injured);
					enemy->TakeDamage(1);
				}
			}
		}	
	}
}

