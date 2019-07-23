#include "pch.h"
#include "CaptainSwimming.h"



void CaptainSwimming::Enter(Captain& cap, State fromState, Data&& data)
{
	cap.vel.x = WATER_FLOW_SPEED;
	cap.vel.y = 0;
}

Data CaptainSwimming::Exit(Captain& cap, State toState)
{
	return Data{};
}

void CaptainSwimming::OnKeyUp(Captain& cap, BYTE keyCode)
{
}

void CaptainSwimming::OnKeyDown(Captain& cap, BYTE keyCode)
{
	if (keyCode == setting.Get(KeyControls::Jump))
	{
		cap.SetState(State::Captain_Jumping);
		return;
	}
}

void CaptainSwimming::Update(Captain& cap, float dt, const std::vector<GameObject*>& coObjects)
{
	auto animation = &cap.animations.at(cap.curState);
	animation->EraseCusFrameHoldTime(0);

	if (wnd.IsKeyPressed(setting.Get(KeyControls::Left)))
	{
		cap.vel.x = -CAP_SWIMMING_SPEED + WATER_FLOW_SPEED;
		cap.nx = -1;
	}
	else if (wnd.IsKeyPressed(setting.Get(KeyControls::Right)))
	{
		cap.vel.x = CAP_SWIMMING_SPEED + WATER_FLOW_SPEED;
		cap.nx = 1;
	}
	else if (wnd.IsKeyPressed(setting.Get(KeyControls::Down)))
	{
		cap.SetState(State::Captain_InWater);
	}
	else
	{
		cap.vel.x = WATER_FLOW_SPEED;
		animation->SetCusFrameHoldTime(0, 0.0f);
	}

	cap.pos.x += cap.vel.x*dt;
}

void CaptainSwimming::HandleCollisions(Captain& cap, float dt, const std::vector<GameObject*>& coObjects)
{
}
