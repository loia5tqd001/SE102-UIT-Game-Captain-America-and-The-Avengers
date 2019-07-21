#include "pch.h"
#include "CaptainWalking.h"



void CaptainWalking::Enter(Captain& cap, State fromState, Data&& data)
{
}

Data CaptainWalking::Exit(Captain& cap, State toState)
{
	return Data{};
}

void CaptainWalking::OnKeyUp(Captain& cap, BYTE keyCode)
{
}

void CaptainWalking::OnKeyDown(Captain& cap, BYTE keyCode)
{
}

void CaptainWalking::Update(Captain& cap, float dt, const std::vector<GameObject*>& coObjects)
{
}

void CaptainWalking::HandleCollisions(Captain& cap, float dt, const std::vector<GameObject*>& coObjects)
{
}
