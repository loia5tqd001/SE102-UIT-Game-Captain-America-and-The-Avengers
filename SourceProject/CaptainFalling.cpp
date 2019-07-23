#include "pch.h"
#include "CaptainFalling.h"

void CaptainFalling::HandleNoCollisions(Captain & cap, float dt)
{
	cap.pos.x += cap.vel.x*dt;
	cap.pos.y += cap.vel.y*dt;
}

void CaptainFalling::Enter(Captain & cap, State fromState, Data && data)
{
	//Todo: Check if Cap is already on the ground or brick or something, switch to another state immediately
	cap.vel.y = FALL_SPEED_VER;
	cap.shield->SetState(State::Invisible);
}

Data CaptainFalling::Exit(Captain & cap, State toState)
{
	return Data();
}

void CaptainFalling::OnKeyUp(Captain & cap, BYTE keyCode)
{
	if (keyCode == setting.Get(KeyControls::Left))
	{
		if (wnd.IsKeyPressed(setting.Get(KeyControls::Right)))
		{
			cap.vel.x = FALL_SPEED_HOR;
			cap.nx = 1;
		}
		else
			cap.vel.x = 0;
		return;
	}

	if (keyCode == setting.Get(KeyControls::Right))
	{
		if (wnd.IsKeyPressed(setting.Get(KeyControls::Left)))
		{
			cap.vel.x = -FALL_SPEED_HOR;
			cap.nx = -1;
		}
		else
			cap.vel.x = 0;
		return;
	}
}

void CaptainFalling::OnKeyDown(Captain & cap, BYTE keyCode)
{
	if (keyCode == setting.Get(KeyControls::Left))
	{
		if (cap.vel.x >= 0)
		{
			cap.vel.x = -FALL_SPEED_HOR;
			cap.nx = -1;
		}
	}

	if (keyCode == setting.Get(KeyControls::Right))
	{
		if (cap.vel.x <= 0)
		{
			cap.vel.x = FALL_SPEED_HOR;
			cap.nx = 1;
		}
	}

}

void CaptainFalling::Update(Captain & cap, float dt, const std::vector<GameObject*>& coObjects)
{
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
	//Testing
	HandleCollisions(cap, dt, coObjects);
	//Endtesting
}

void CaptainFalling::HandleCollisions(Captain & cap, float dt, const std::vector<GameObject*>& coObjects)
{
	auto coEvents = CollisionDetector::CalcPotentialCollisions(cap, coObjects, dt);
	if (coEvents.size() == 0) { HandleNoCollisions(cap, dt); return; }

	float min_tx, min_ty, nx, ny;
	CollisionDetector::FilterCollisionEvents(coEvents, min_tx, min_ty, nx, ny);

	if (coEvents.size() == 0) return;

	cap.pos.x += min_tx * cap.vel.x * dt;
	cap.pos.y += min_ty * cap.vel.y * dt;

	if (nx != 0.0f) cap.vel.x = 0.0f;
	if (ny < 0.0f) { cap.vel.y = 0.0f; cap.SetState(State::Captain_Standing); }

	//// Collision logic with Goombas
	//for (UINT i = 0; i < coEvents.size(); i++)
	//{
	//	const CollisionEvent& e = coEvents[i];

	//	if (auto goomba = dynamic_cast<Goomba*>(e.pCoObj))
	//	{
	//		if (e.ny < 0.0f && goomba->GetState() != State::GoombaDie)
	//		{
	//			goomba->SetState(State::GoombaDie);
	//			vel.y = -JUMP_DEFLECT_SPEED;
	//			OnFlashing(true);
	//		}

	//		if (e.ny == 0.0f)
	//		{
	//			pos = posbak;
	//		}
	//	}
	//}
}
