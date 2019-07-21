#include "pch.h"
#include "CaptainKicking.h"



void CaptainKicking::Enter(Captain& cap, State fromState, Data&& data)
{
}

Data CaptainKicking::Exit(Captain& cap, State toState)
{
	return Data{};
}

void CaptainKicking::OnKeyUp(Captain& cap, BYTE keyCode)
{
}

void CaptainKicking::OnKeyDown(Captain& cap, BYTE keyCode)
{
}

void CaptainKicking::Update(Captain& cap, float dt, const std::vector<GameObject*>& coObjects)
{
}

void CaptainKicking::HandleCollisions(Captain& cap, float dt, const std::vector<GameObject*>& coObjects)
{
}


