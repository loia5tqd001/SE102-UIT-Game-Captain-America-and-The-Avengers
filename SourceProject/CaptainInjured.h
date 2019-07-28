#pragma once
#include "CaptainState.h"

class CaptainInjured : public CaptainState
{
	State pendingSwitchState;
	float posxWhenGotInjure;
	float DistanceLeftToClimb;
	float maxTimeHold = 0;
	void HandleNoCollisions(Captain &cap, float dt);
	Vector2 prevPos = Vector2{ 0,0 };
public:
	void Enter(Captain& cap, State fromState, Data&& data) override;
	Data Exit(Captain& cap, State toState) override;
	void OnKeyUp(Captain& cap, BYTE keyCode) override;
	void OnKeyDown(Captain& cap, BYTE keyCode) override;
	void Update(Captain& cap, float dt, const std::vector<GameObject*>& coObjects) override;
	void HandleCollisions(Captain& cap, float dt, const std::vector<GameObject*>& coObjects) override;
};

