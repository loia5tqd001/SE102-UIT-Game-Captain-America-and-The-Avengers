#include "pch.h"
#include "CaptainClimbing.h"



void CaptainClimbing::Enter(Captain& cap, State fromState, Data&& data)
{
	//Todo: set position base on Jump Exit data
	cap.vel.x = 0.0f;
	cap.vel.y = 0.0f;
}

Data CaptainClimbing::Exit(Captain& cap, State toState)
{
	return Data();
}

void CaptainClimbing::OnKeyUp(Captain& cap, BYTE keyCode)
{
}

void CaptainClimbing::OnKeyDown(Captain& cap, BYTE keyCode)
{
	if (keyCode == setting.Get(KeyControls::Left))
	{
		if (cap.nx == -1)
		{
			return;
		}
		else
		{
			cap.nx = -1;
			cap.pos.x = cap.pos.x + cap.GetBBox().GetWidth() / 2;
		}
	}
	else if (keyCode == setting.Get(KeyControls::Right))
	{
		if (cap.nx == 1)
		{
			return;
		}
		else
		{
			cap.pos.x = cap.pos.x - cap.GetBBox().GetWidth() / 2;
			cap.nx = 1;
		}
	}
	else if (keyCode == setting.Get(KeyControls::Down))
	{
		//Todo: upgrade this
		cap.SetState(State::Captain_Jumping);
		cap.vel.y = std::abs(cap.vel.y);
	}
	else if (keyCode==setting.Get(KeyControls::Jump))
	{
		cap.SetState(State::Captain_Jumping);
	}
}

void CaptainClimbing::Update(Captain& cap, float dt, const std::vector<GameObject*>& coObjects)
{
}

void CaptainClimbing::HandleCollisions(Captain& cap, float dt, const std::vector<GameObject*>& coObjects)
{
}

