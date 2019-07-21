#include "pch.h"
#include "CaptainTackle.h"



void CaptainTackle::Enter(Captain& cap, State fromState, Data&& data)
{
}

Data CaptainTackle::Exit(Captain& cap, State toState)
{
	return Data{};
}

void CaptainTackle::OnKeyUp(Captain& cap, BYTE keyCode)
{
}

void CaptainTackle::OnKeyDown(Captain& cap, BYTE keyCode)
{
}

void CaptainTackle::Update(Captain& cap, float dt, const std::vector<GameObject*>& coObjects)
{
}

void CaptainTackle::HandleCollisions(Captain& cap, float dt, const std::vector<GameObject*>& coObjects)
{
}


