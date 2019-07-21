#include "pch.h"
#include "CaptainPunching.h"



void CaptainPunching::Enter(Captain& cap, State fromState, Data&& data)
{
}

Data CaptainPunching::Exit(Captain& cap, State toState)
{
	return Data{};
}

void CaptainPunching::OnKeyUp(Captain& cap, BYTE keyCode)
{
}

void CaptainPunching::OnKeyDown(Captain& cap, BYTE keyCode)
{
}

void CaptainPunching::Update(Captain& cap, float dt, const std::vector<GameObject*>& coObjects)
{
}

void CaptainPunching::HandleCollisions(Captain& cap, float dt, const std::vector<GameObject*>& coObjects)
{
}


