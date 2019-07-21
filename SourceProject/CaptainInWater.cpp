#include "pch.h"
#include "CaptainInWater.h"



void CaptainInWater::Enter(Captain& cap, State fromState, Data&& data)
{
}

Data CaptainInWater::Exit(Captain& cap, State toState)
{
	return Data{};
}

void CaptainInWater::OnKeyUp(Captain& cap, BYTE keyCode)
{
}

void CaptainInWater::OnKeyDown(Captain& cap, BYTE keyCode)
{
}

void CaptainInWater::Update(Captain& cap, float dt, const std::vector<GameObject*>& coObjects)
{
}

void CaptainInWater::HandleCollisions(Captain& cap, float dt, const std::vector<GameObject*>& coObjects)
{
}


