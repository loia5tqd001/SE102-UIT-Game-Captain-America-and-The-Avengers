#include "pch.h"
#include "CaptainDead.h"



void CaptainDead::Enter(Captain& cap, State fromState, Data&& data)
{
	Sounds::PlayAt(SoundId::Death);
	cap.vel.x = 0.0f;
	cap.vel.y = 50.0f;
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
	HandleCollisions(cap, dt, coObjects);
}

void CaptainDead::HandleCollisions(Captain& cap, float dt, const std::vector<GameObject*>& coObjects)
{
	auto coEvents = CollisionDetector::CalcPotentialCollisions(cap, coObjects, dt);

	float min_tx, min_ty, nx, ny;
	CollisionDetector::FilterCollisionEvents(coEvents, min_tx, min_ty, nx, ny);

	if (coEvents.size() == 0) return;
	cap.pos.x += min_tx * cap.vel.x * dt;
	cap.pos.y += min_ty * cap.vel.y * dt;
}

