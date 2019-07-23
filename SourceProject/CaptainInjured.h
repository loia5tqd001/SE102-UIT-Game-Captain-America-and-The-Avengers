#pragma once
#include "CaptainState.h"

class CaptainInjured : public CaptainState
{
	float posxWhenGotInjure;
	float waterLevel;
	void HandleNoCollisions(Captain &cap, float dt);
public:
	void Enter(Captain& cap, State fromState, Data&& data) override;
	Data Exit(Captain& cap, State toState) override;
	void OnKeyUp(Captain& cap, BYTE keyCode) override;
	void OnKeyDown(Captain& cap, BYTE keyCode) override;
	void Update(Captain& cap, float dt, const std::vector<GameObject*>& coObjects) override;
	void HandleCollisions(Captain& cap, float dt, const std::vector<GameObject*>& coObjects) override;
};

