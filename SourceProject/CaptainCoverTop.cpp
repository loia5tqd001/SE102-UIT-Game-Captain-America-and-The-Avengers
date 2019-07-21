#include "pch.h"
#include "CaptainCoverTop.h"



void CaptainCoverTop::Enter(Captain& cap, State fromState, Data&& data)
{
}

Data CaptainCoverTop::Exit(Captain& cap, State toState)
{
	return Data{};
}

void CaptainCoverTop::OnKeyUp(Captain& cap, BYTE keyCode)
{
}

void CaptainCoverTop::OnKeyDown(Captain& cap, BYTE keyCode)
{
}

void CaptainCoverTop::Update(Captain& cap, float dt, const std::vector<GameObject*>& coObjects)
{
}

void CaptainCoverTop::HandleCollisions(Captain& cap, float dt, const std::vector<GameObject*>& coObjects)
{
}


