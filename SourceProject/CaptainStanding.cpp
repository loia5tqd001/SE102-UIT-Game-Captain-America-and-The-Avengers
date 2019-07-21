#include "pch.h"
#include "CaptainStanding.h"



void CaptainStanding::Enter(Captain& cap, State fromState, Data&& data)
{
	cap.vel.x = cap.vel.y = 0;
}

Data CaptainStanding::Exit(Captain& cap, State toState)
{
	return Data{};
}

void CaptainStanding::OnKeyUp(Captain& cap, BYTE keyCode)
{
}

void CaptainStanding::OnKeyDown(Captain& cap, BYTE keyCode)
{
	std::optional<State> shouldBeNextState;

	if (keyCode == setting.Get(KeyControls::Left)) {
		cap.vel.x -= WALKING_SPEED;
	}
	if (keyCode == setting.Get(KeyControls::Right)) {
		cap.vel.x += WALKING_SPEED;
	}
	if (cap.vel.x != 0.0f) {
		shouldBeNextState = State::Captain_Walking;
	}
	else {
		int yDir = 0;
		if (keyCode == setting.Get(KeyControls::Up)) {
			yDir --;
		}
		if (keyCode == setting.Get(KeyControls::Down)) {
			yDir ++;
		}
		if (yDir != 0) {
			shouldBeNextState = yDir > 0 ? State::Captain_Sitting : State::Captain_CoverTop;
		}
	}

	if (shouldBeNextState.has_value())
	{
		cap.SetState(*shouldBeNextState);
	}
}

void CaptainStanding::Update(Captain& cap, float dt, const std::vector<GameObject*>& coObjects)
{
	// when standing, almost do nothin, only respond to collisions
	HandleCollisions(cap, dt, coObjects);
}

void CaptainStanding::HandleCollisions(Captain& cap, float dt, const std::vector<GameObject*>& coObjects)
{
}
