#include "pch.h"
#include "CaptainInjured.h"

void CaptainInjured::Enter(Captain& cap, State fromState, Data&& data)
{
}

Data CaptainInjured::Exit(Captain& cap, State toState)
{
	return Data{};
}

void CaptainInjured::OnKeyUp(Captain& cap, BYTE keyCode)
{
}

void CaptainInjured::OnKeyDown(Captain& cap, BYTE keyCode)
{
}

void CaptainInjured::Update(Captain& cap, float dt, const std::vector<GameObject*>& coObjects)
{
}

void CaptainInjured::HandleCollisions(Captain& cap, float dt, const std::vector<GameObject*>& coObjects)
{
}

