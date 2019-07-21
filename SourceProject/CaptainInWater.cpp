#include "pch.h"
#include "CaptainInWater.h"



void CaptainInWater::Enter(Captain& cap, State fromState, Data&& data)
{
	cap.vel.x = WATER_FLOW_SPEED;
	cap.vel.y = 0.0f;
}

Data CaptainInWater::Exit(Captain& cap, State toState)
{
	return Data{};
}

void CaptainInWater::OnKeyUp(Captain& cap, BYTE keyCode)
{
}

void CaptainInWater::OnKeyDown(Captain& cap, BYTE keyCode)
{
	if (keyCode==setting.Get(KeyControls::Jump))
	{
		cap.SetState(State::Captain_Jumping);
		return;
	}
}

void CaptainInWater::Update(Captain& cap, float dt, const std::vector<GameObject*>& coObjects)
{
	if (!wnd.IsKeyPressed(setting.Get(KeyControls::Down)))
	{
		cap.SetState(State::Captain_Swimming);
		return;
	}
	cap.pos.x += cap.vel.x*dt;
}

void CaptainInWater::HandleCollisions(Captain& cap, float dt, const std::vector<GameObject*>& coObjects)
{
}


