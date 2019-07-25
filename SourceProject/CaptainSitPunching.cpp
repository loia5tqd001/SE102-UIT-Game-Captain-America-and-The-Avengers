#include "pch.h"
#include "CaptainSitPunching.h"
#include "BulletEnemyRocket.h"



void CaptainSitPunching::Enter(Captain& cap, State fromState, Data&& data)
{
	assert(fromState == State::Captain_Sitting || fromState == State::Captain_CoverLow);
	cap.vel.y = 0;
	cap.vel.x = 0;
	if (fromState == State::Captain_Sitting)
	{
		// adjust position due to difference between sitting witdh and sitpunching width
		auto sittingWidth = cap.animations.at(State::Captain_Sitting).GetFrameSize().GetWidth();
		auto sitpunchingWidth = cap.animations.at(State::Captain_SitPunching).GetFrameSize().GetWidth();
		dentaX = (float(sittingWidth) - float(sitpunchingWidth)) / 2;
		if (cap.nx < 0) {
			cap.pos.x += dentaX;
		}
		else {
			cap.pos.x -= dentaX;
		}
	}
}

Data CaptainSitPunching::Exit(Captain& cap, State toState)
{
	Sounds::PlayAt(SoundId::Punch);
	if (toState == State::Captain_Sitting)
	{
		if (cap.nx < 0) {
			cap.pos.x -= dentaX;
		}
		else {
			cap.pos.x += dentaX;
		}
	}
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
	if (cap.animations.at(cap.curState).IsDoneCycle()) {

		cap.SetState(State::Captain_Sitting); //move to Captain_Sitting, Captain_Walking should handle nx, KeyControls press, up and down
	}
	HandleCollisions(cap, dt, coObjects);
}

void CaptainSitPunching::HandleCollisions(Captain& cap, float dt, const std::vector<GameObject*>& coObjects)
{
	auto coEvents = CollisionDetector::CalcPotentialCollisions(cap, coObjects, dt);

	float min_tx, min_ty, nx, ny;

	if (coEvents.size() == 0) return;
	CollisionDetector::FilterCollisionEvents(coEvents, min_tx, min_ty, nx, ny);

	for (auto& e : coEvents)
	{
	    if (auto bullet = dynamic_cast<Bullet*>(e.pCoObj))
		{
			if (!cap.isFlashing)
			{
				cap.health.Subtract(bullet->GetDamage());
				bullet->HitCaptain();
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
				if (nx * e.nx < 0)
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

