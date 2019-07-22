#include "pch.h"
#include "CaptainTackle.h"



void CaptainTackle::Enter(Captain& cap, State fromState, Data&& data)
{
	cap.vel.x = cap.nx * TACKLE_SPEED;
	Sounds::PlayAt(SoundId::Tackle);
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
	else
	{
		auto kControlDir = cap.nx > 0 ? KeyControls::Right : KeyControls::Left;
		if (!wnd.IsKeyPressed(setting.Get(kControlDir)))
		{
			std::chrono::duration<float> duration = std::chrono::steady_clock::now() - cap.timeLastKeyUp;
			if (duration.count() > 0.1f) {
				cap.SetState(State::Captain_Standing);
			}
		}
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


