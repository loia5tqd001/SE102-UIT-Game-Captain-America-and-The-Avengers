#include "pch.h"
#include "CaptainThrowing.h"



void CaptainThrowing::Enter(Captain& cap, State fromState, Data&& data)
{
}

Data CaptainThrowing::Exit(Captain& cap, State toState)
{
	return Data{};
}

void CaptainThrowing::OnKeyUp(Captain& cap, BYTE keyCode)
{
}

void CaptainThrowing::OnKeyDown(Captain& cap, BYTE keyCode)
{
}

void CaptainThrowing::Update(Captain& cap, float dt, const std::vector<GameObject*>& coObjects)
{
}

void CaptainThrowing::HandleCollisions(Captain& cap, float dt, const std::vector<GameObject*>& coObjects)
{
}

