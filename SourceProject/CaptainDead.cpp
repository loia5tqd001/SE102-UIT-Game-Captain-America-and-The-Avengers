#include "pch.h"
#include "CaptainDead.h"



void CaptainDead::Enter(Captain& cap, State fromState, Data&& data)
{
	Sounds::PlayAt(SoundId::Death);
}

Data CaptainDead::Exit(Captain& cap, State toState)
{
	return Data{};
}

void CaptainDead::OnKeyUp(Captain& cap, BYTE keyCode)
{
}

void CaptainDead::OnKeyDown(Captain& cap, BYTE keyCode)
{
}

void CaptainDead::Update(Captain& cap, float dt, const std::vector<GameObject*>& coObjects)
{
	if (cap.animations.at(cap.curState).IsDoneCycle())
	{
		sceneManager.GetCurScene().DoTransitionScene();
		cap.health.Set(12);
		cap.SetState(State::Captain_Standing);
	}
}

void CaptainDead::HandleCollisions(Captain& cap, float dt, const std::vector<GameObject*>& coObjects)
{
}

