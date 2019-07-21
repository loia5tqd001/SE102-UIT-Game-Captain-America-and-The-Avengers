#include "pch.h"
#include "CaptainSitting.h"



void CaptainSitting::Enter(Captain& cap, State fromState, Data&& data)
{
}

Data CaptainSitting::Exit(Captain& cap, State toState)
{
	return Data{};
}

void CaptainSitting::OnKeyUp(Captain& cap, BYTE keyCode)
{
}

void CaptainSitting::OnKeyDown(Captain& cap, BYTE keyCode)
{
}

void CaptainSitting::Update(Captain& cap, float dt, const std::vector<GameObject*>& coObjects)
{
}

void CaptainSitting::HandleCollisions(Captain& cap, float dt, const std::vector<GameObject*>& coObjects)
{
}


