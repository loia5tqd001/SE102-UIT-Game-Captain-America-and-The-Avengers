#include "pch.h"
#include "CaptainFallToWater.h"



void CaptainFallToWater::Enter(Captain& cap, State fromState, Data&& data)
{
}

Data CaptainFallToWater::Exit(Captain& cap, State toState)
{
	return Data{};
}

void CaptainFallToWater::OnKeyUp(Captain& cap, BYTE keyCode)
{
}

void CaptainFallToWater::OnKeyDown(Captain& cap, BYTE keyCode)
{
}

void CaptainFallToWater::Update(Captain& cap, float dt, const std::vector<GameObject*>& coObjects)
{
}

void CaptainFallToWater::HandleCollisions(Captain& cap, float dt, const std::vector<GameObject*>& coObjects)
{
}
