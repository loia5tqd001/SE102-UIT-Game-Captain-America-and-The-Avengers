#include "pch.h"
#include "CaptainKicking.h"



void CaptainKicking::Enter(Captain& cap, State fromState, Data&& data)
{
	assert(fromState == State::Captain_Jumping || fromState == State::Captain_Spinning);
	lastState = fromState;
	if (fromState == State::Captain_Jumping)
	{
		isJumpReleased = data.Get<bool>(IS_JUMP_RELEASED);
	    JumpHeightRealCounter = data.Get<float>(JUMP_HEIGHT_RealCounter);
		JumpHeightNeedCounter = data.Get<float>(JUMP_HEIGHT_NeedCounter);
	}
}

Data CaptainKicking::Exit(Captain& cap, State toState)
{
	Data data;
	switch (toState)
	{
	case State::Captain_Kicking:
		data.Add(IS_JUMP_RELEASED, isJumpReleased);
		data.Add(JUMP_HEIGHT_RealCounter, JumpHeightRealCounter);
		data.Add(JUMP_HEIGHT_NeedCounter, JumpHeightNeedCounter);
		break;
	}
	return std::move(data);
}

void CaptainKicking::OnKeyUp(Captain& cap, BYTE keyCode)
{
	if (keyCode == setting.Get(KeyControls::Jump))
		isJumpReleased = true;
}

void CaptainKicking::OnKeyDown(Captain& cap, BYTE keyCode)
{
}

void CaptainKicking::Update(Captain& cap, float dt, const std::vector<GameObject*>& coObjects)
{
	if (cap.animations.at(cap.curState).IsDoneCycle())
	{
		if (lastState == State::Captain_Jumping)
			cap.SetState(lastState);
		else if (lastState == State::Captain_Spinning)
			cap.SetState(State::Captain_Spinning); //fix
	}
	if (wnd.IsKeyPressed(setting.Get(KeyControls::Left)))
	{
		cap.vel.x = MOVING_HOR;
	}
	if (wnd.IsKeyPressed(setting.Get(KeyControls::Right)))
	{
		cap.vel.x = -MOVING_HOR;
	}

	if (JumpHeightNeedCounter < MAX_JUMP_HEIGHT) {
		if (!isJumpReleased) {
			JumpHeightNeedCounter += JUMP_SPEED * dt;
			cap.vel.y = -JUMP_SPEED;
			JumpHeightRealCounter += JUMP_SPEED * dt;
		}
		else {
			if (JumpHeightRealCounter < JumpHeightNeedCounter)
			{
				cap.vel.y = -JUMP_SPEED;
				JumpHeightRealCounter += JUMP_SPEED * dt;
			}
			else
			{
				if (lastState == State::Captain_Jumping)
					cap.SetState(lastState);
				else if (lastState == State::Captain_Spinning)
					cap.SetState(State::Captain_Spinning); //fix
			}
		}
	}
	else {
		if (JumpHeightRealCounter < JumpHeightNeedCounter)
		{
			cap.vel.y = -JUMP_SPEED;
			JumpHeightRealCounter += JUMP_SPEED * dt;
		}
		else
		{
		}
	}

	HandleCollisions(cap, dt, coObjects);
}

void CaptainKicking::HandleCollisions(Captain& cap, float dt, const std::vector<GameObject*>& coObjects)
{
	// collision with captain kick
	cap.pos.x += cap.vel.x*dt;
	cap.pos.y += cap.vel.y*dt;
}


