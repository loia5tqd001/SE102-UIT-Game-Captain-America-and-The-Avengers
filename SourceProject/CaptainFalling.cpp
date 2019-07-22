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
	//Testing
	HandleNoCollisions(cap, dt);
	//Endtesting
}

void CaptainFalling::HandleCollisions(Captain & cap, float dt, const std::vector<GameObject*>& coObjects)
{
}
