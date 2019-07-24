#include "pch.h"
#include "CaptainStanding.h"



void CaptainStanding::Enter(Captain& cap, State fromState, Data&& data)
{
	cap.vel.x;
	cap.vel.y = 170.0f;
	isToSittingTackle = false;
}

Data CaptainStanding::Exit(Captain& cap, State toState)
{
	Data data;
	switch (toState)
	{
		case State::Captain_Sitting:
			data.Add(IS_TO_SIT_TACKLE, isToSittingTackle);
			break;
	}

	return std::move(data);
}

void CaptainStanding::OnKeyUp(Captain& cap, BYTE keyCode)
{
}

void CaptainStanding::OnKeyDown(Captain& cap, BYTE keyCode)
{
	auto kControlDir = cap.nx > 0 ? KeyControls::Right : KeyControls::Left;

	// if pressdown two time in a row in direction
	if (setting.Get(kControlDir) == keyCode &&
	    cap.lastKeyDown == kControlDir && cap.lastKeyUp == kControlDir)
	{
		std::chrono::duration<float> duration = std::chrono::steady_clock::now() - cap.timeLastKeyDown;
		if (duration.count() < 0.15f)
		{
			isToSittingTackle = true;
			cap.SetState(State::Captain_Sitting);
			return;
		}
	}
}

void CaptainStanding::Update(Captain& cap, float dt, const std::vector<GameObject*>& coObjects)
{
	if (wnd.IsKeyPressed(setting.Get(KeyControls::Down)))
	{
		cap.SetState(State::Captain_Sitting);
	}
	else if (wnd.IsKeyPressed(setting.Get(KeyControls::Up)))
	{
		cap.SetState(State::Captain_CoverTop);
	}
	else if (wnd.IsKeyPressed(setting.Get(KeyControls::Jump)))
	{
		cap.SetState(State::Captain_Jumping);
	}
	else if (wnd.IsKeyPressed(setting.Get(KeyControls::Attack)))
	{
		if (cap.shieldOn) {
			cap.SetState(State::Captain_Throwing);
		}
		else {
			cap.SetState(State::Captain_Punching);
		}
	}
	else
	{
		int dir = 0;
		if (wnd.IsKeyPressed( setting.Get(KeyControls::Left) ))
		{
			dir --;
		}
		if (wnd.IsKeyPressed( setting.Get(KeyControls::Right) ))
		{
			dir ++;
			cap.nx = 1;
		}
		if (dir != 0)
		{
			cap.nx = dir;
			cap.SetState(State::Captain_Walking);
		}
	}

	HandleCollisions(cap, dt, coObjects);
}

void CaptainStanding::HandleCollisions(Captain& cap, float dt, const std::vector<GameObject*>& coObjects)
{
}
