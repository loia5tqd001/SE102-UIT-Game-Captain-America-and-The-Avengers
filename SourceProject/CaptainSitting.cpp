#include "pch.h"
#include "CaptainSitting.h"



void CaptainSitting::Enter(Captain& cap, State fromState, Data&& data)
{
	cap.vel.x = cap.vel.y = 0.0f;
	switch (fromState)
	{
		case State::Captain_Walking:
		case State::Captain_Standing:
			if (data.Count(IS_TO_SIT_TACKLE))
			{
				isSitToTackle = data.Get<bool>(IS_TO_SIT_TACKLE);
			}
			break;
		default:
			isSitToTackle = false;
			break;
	}

}

Data CaptainSitting::Exit(Captain& cap, State toState)
{
	return Data{};
}

void CaptainSitting::OnKeyUp(Captain& cap, BYTE keyCode)
{
	//if (setting.IsKeyControl(keyCode))
	//{
	//	cap.SetState(State::Captain_Standing);
	//}
}

void CaptainSitting::OnKeyDown(Captain& cap, BYTE keyCode)
{
	int dir = 0;
	if (keyCode == setting.Get(KeyControls::Left))
	{
		dir --;
	}
	else if (keyCode == setting.Get(KeyControls::Right))
	{
		dir ++;
	}
	else if (keyCode == setting.Get(KeyControls::Attack))
	{
		cap.SetState(State::Captain_SitPunching);
	}
	else if (keyCode == setting.Get(KeyControls::Jump) && !wnd.IsKeyPressed(setting.Get(KeyControls::Down)))
	{
		cap.SetState(State::Captain_Jumping);
	}

	if (dir != 0)
	{
		cap.nx = dir;
		cap.SetState(State::Captain_Walking);
	}
}

void CaptainSitting::Update(Captain& cap, float dt, const std::vector<GameObject*>& coObjects)
{
	if (cap.animations.at(cap.curState).IsDoneCycle())
	{
		if (isSitToTackle)
		{
			cap.SetState(State::Captain_Tackle);
		}
		else if (!wnd.IsKeyPressed(setting.Get(KeyControls::Down)))
		{
			cap.SetState(State::Captain_Standing);
		}
	}
	if (wnd.IsKeyPressed(setting.Get(KeyControls::Jump)) && wnd.IsKeyPressed(setting.Get(KeyControls::Down)))
	{
		cap.SetState(State::Captain_Falling);
		cap.pos.y += 1.0f;
	}
}

void CaptainSitting::HandleCollisions(Captain& cap, float dt, const std::vector<GameObject*>& coObjects)
{
}


