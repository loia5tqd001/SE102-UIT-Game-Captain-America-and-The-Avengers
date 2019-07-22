#include "pch.h"
#include "CaptainKicking.h"



void CaptainKicking::Enter(Captain& cap, State fromState, Data&& data)
{
	assert(fromState == State::Captain_Jumping || fromState == State::Captain_Spinning);
	lastState = fromState;
	if (fromState == State::Captain_Jumping)
	{
		isJumpReleased = data.Get<bool>(IS_JUMP_RELEASED);
		canHigher = data.Get<bool>(CAN_JUMP_HIGHER);
	}
}

Data CaptainKicking::Exit(Captain& cap, State toState)
{
	return Data{};
}

void CaptainKicking::OnKeyUp(Captain& cap, BYTE keyCode)
{
}

void CaptainKicking::OnKeyDown(Captain& cap, BYTE keyCode)
{
}

void CaptainKicking::Update(Captain& cap, float dt, const std::vector<GameObject*>& coObjects)
{
	if (cap.animations.at(cap.curState).IsDoneCycle())
	{
		cap.SetState(lastState);
	}
	cap.vel.x = 0.0f;
	if (wnd.IsKeyPressed(setting.Get(KeyControls::Left)))
	{
		cap.vel.x -= 10.0f;
	}
	if (wnd.IsKeyPressed(setting.Get(KeyControls::Right)))
	{
		cap.vel.x += 10.0f;
	}
	HandleCollisions(cap, dt, coObjects);
}

void CaptainKicking::HandleCollisions(Captain& cap, float dt, const std::vector<GameObject*>& coObjects)
{
	// collision with captain kick
	cap.pos.x += cap.vel.x;
	cap.pos.y += cap.vel.y;
}


