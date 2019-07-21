#include "pch.h"
#include "CaptainSwimming.h"



void CaptainSwimming::Enter(Captain& cap, State fromState, Data&& data)
{
}

Data CaptainSwimming::Exit(Captain& cap, State toState)
{
	return Data{};
}

void CaptainSwimming::OnKeyUp(Captain& cap, BYTE keyCode)
{
}

void CaptainSwimming::OnKeyDown(Captain& cap, BYTE keyCode)
{
}

void CaptainSwimming::Update(Captain& cap, float dt, const std::vector<GameObject*>& coObjects)
{
}

void CaptainSwimming::HandleCollisions(Captain& cap, float dt, const std::vector<GameObject*>& coObjects)
{
}
