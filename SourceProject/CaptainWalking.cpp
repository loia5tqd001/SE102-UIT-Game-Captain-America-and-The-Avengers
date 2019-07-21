#include "pch.h"
#include "CaptainWalking.h"



void CaptainWalking::Enter(Captain& cap, State fromState, Data&& data)
{
	cap.vel.x = cap.nx * WALKING_SPEED;
}

Data CaptainWalking::Exit(Captain& cap, State toState)
{
	return Data{};
}

void CaptainWalking::OnKeyUp(Captain& cap, BYTE keyCode)
{
	auto kControlDir = cap.nx > 0 ? KeyControls::Right : KeyControls::Left;
	if (keyCode == setting.Get(kControlDir))
	{
		cap.SetState(State::Captain_Standing);
	}
}

void CaptainWalking::OnKeyDown(Captain& cap, BYTE keyCode)
{
}

void CaptainWalking::Update(Captain& cap, float dt, const std::vector<GameObject*>& coObjects)
{
	cap.pos.x += cap.vel.x;
}

void CaptainWalking::HandleCollisions(Captain& cap, float dt, const std::vector<GameObject*>& coObjects)
{
}
