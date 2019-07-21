#include "pch.h"
#include "CaptainSpinning.h"



void CaptainSpinning::Enter(Captain& cap, State fromState, Data&& data)
{
}

Data CaptainSpinning::Exit(Captain& cap, State toState)
{
	return Data{};
}

void CaptainSpinning::OnKeyUp(Captain& cap, BYTE keyCode)
{
}

void CaptainSpinning::OnKeyDown(Captain& cap, BYTE keyCode)
{
}

void CaptainSpinning::Update(Captain& cap, float dt, const std::vector<GameObject*>& coObjects)
{
}

void CaptainSpinning::HandleCollisions(Captain& cap, float dt, const std::vector<GameObject*>& coObjects)
{
}

