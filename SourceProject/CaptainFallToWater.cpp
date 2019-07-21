#include "pch.h"
#include "CaptainFallToWater.h"



void CaptainFallToWater::Enter(Captain& cap, State fromState, Data&& data)
{
	//Todo: posx.y = .... Cap's positon must be underwater
	cap.vel.y = 0.0f;
}

Data CaptainFallToWater::Exit(Captain& cap, State toState)
{
	return Data{};
}

void CaptainFallToWater::OnKeyUp(Captain& cap, BYTE keyCode)
{
}

void CaptainFallToWater::OnKeyDown(Captain& cap, BYTE keyCode)
{
	//Jump out the water
	if (keyCode==setting.Get(KeyControls::Jump))
	{
		cap.SetState(State::Captain_Jumping);
	}
}

void CaptainFallToWater::Update(Captain& cap, float dt, const std::vector<GameObject*>& coObjects)
{
	if (cap.animations.at(cap.curState).IsDoneCycle())
	{
		cap.SetState(State::Captain_Swimming);
		return;
	}

	cap.pos.x += WATER_FLOW_SPEED * dt;
}

void CaptainFallToWater::HandleCollisions(Captain& cap, float dt, const std::vector<GameObject*>& coObjects)
{
}
