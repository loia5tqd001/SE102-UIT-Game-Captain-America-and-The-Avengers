#include "pch.h"
#include "CaptainSitting.h"



void CaptainSitting::Enter(Captain& cap, State fromState, Data&& data)
{
	cap.vel.x = cap.vel.y = 0.0f;
	switch (fromState)
	{
		case State::Captain_Standing:
			if (data.Count("is-tackle"))
			{
				isSitToTackle = data.Get<bool>("is-tackle");
				Debug::Out("isSit", isSitToTackle);
			}
			break;
	}
}

Data CaptainSitting::Exit(Captain& cap, State toState)
{
	return Data{};
}

void CaptainSitting::OnKeyUp(Captain& cap, BYTE keyCode)
{
	if (setting.IsKeyControl(keyCode))
	{
		cap.SetState(State::Captain_Standing);
	}
}

void CaptainSitting::OnKeyDown(Captain& cap, BYTE keyCode)
{
}

void CaptainSitting::Update(Captain& cap, float dt, const std::vector<GameObject*>& coObjects)
{
	if (isSitToTackle && cap.animations.at(cap.curState).IsDoneCycle())
	{
		Debug::Out("Tacklee");
		cap.SetState(State::Captain_Tackle);
	}
}

void CaptainSitting::HandleCollisions(Captain& cap, float dt, const std::vector<GameObject*>& coObjects)
{
}


