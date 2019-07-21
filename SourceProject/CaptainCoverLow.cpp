#include "pch.h"
#include "CaptainCoverLow.h"



void CaptainCoverLow::Enter(Captain& cap, State fromState, Data&& data)
{
}

Data CaptainCoverLow::Exit(Captain& cap, State toState)
{
	return Data{};
}

void CaptainCoverLow::OnKeyUp(Captain& cap, BYTE keyCode)
{
}

void CaptainCoverLow::OnKeyDown(Captain& cap, BYTE keyCode)
{
}

void CaptainCoverLow::Update(Captain& cap, float dt, const std::vector<GameObject*>& coObjects)
{
}

void CaptainCoverLow::HandleCollisions(Captain& cap, float dt, const std::vector<GameObject*>& coObjects)
{
}

