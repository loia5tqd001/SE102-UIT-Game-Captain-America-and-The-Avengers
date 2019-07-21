#include "pch.h"
#include "CaptainSitPunching.h"



void CaptainSitPunching::Enter(Captain& cap, State fromState, Data&& data)
{
}

Data CaptainSitPunching::Exit(Captain& cap, State toState)
{
	return Data{};
}

void CaptainSitPunching::OnKeyUp(Captain& cap, BYTE keyCode)
{
}

void CaptainSitPunching::OnKeyDown(Captain& cap, BYTE keyCode)
{
}

void CaptainSitPunching::Update(Captain& cap, float dt, const std::vector<GameObject*>& coObjects)
{
}

void CaptainSitPunching::HandleCollisions(Captain& cap, float dt, const std::vector<GameObject*>& coObjects)
{
}

