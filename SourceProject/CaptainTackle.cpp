#include "pch.h"
#include "CaptainTackle.h"



void CaptainTackle::Enter(Captain& cap, State fromState, Data&& data)
{
	cap.vel.x = cap.nx * TACKLE_SPEED;
}

Data CaptainTackle::Exit(Captain& cap, State toState)
{
	return Data{};
}

void CaptainTackle::OnKeyUp(Captain& cap, BYTE keyCode)
{
}

void CaptainTackle::OnKeyDown(Captain& cap, BYTE keyCode)
{
}

void CaptainTackle::Update(Captain& cap, float dt, const std::vector<GameObject*>& coObjects)
{
	cap.pos.x += cap.vel.x * dt;

	if (cap.animations.at(cap.curState).IsDoneCycle())
	{
		cap.SetState(State::Captain_Standing);
	}

	/*
	if (wnd.IsKeyPressed(setting.Get(kControlDir)))
	{
		Debug::Out("Keep tackle");
	}
	else
	{
		Debug::Out("Done tackle");
	}*/
}

void CaptainTackle::HandleCollisions(Captain& cap, float dt, const std::vector<GameObject*>& coObjects)
{
}


