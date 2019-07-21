#include "pch.h"
#include "CaptainClimbing.h"



void CaptainClimbing::Enter(Captain& cap, State fromState, Data&& data)
{
}

Data CaptainClimbing::Exit(Captain& cap, State toState)
{
	return Data{};
}

void CaptainClimbing::OnKeyUp(Captain& cap, BYTE keyCode)
{
}

void CaptainClimbing::OnKeyDown(Captain& cap, BYTE keyCode)
{
}

void CaptainClimbing::Update(Captain& cap, float dt, const std::vector<GameObject*>& coObjects)
{
}

void CaptainClimbing::HandleCollisions(Captain& cap, float dt, const std::vector<GameObject*>& coObjects)
{
}

