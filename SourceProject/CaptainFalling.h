#pragma once
#include "CaptainState.h"
class CaptainFalling :	public CaptainState
{
private:
	float waterLevel;
	void HandleNoCollisions(Captain& cap, float dt);

	bool isKicked = false;
public:
	void Enter(Captain& cap, State fromState, Data&& data = {}) override; // handle when change to this state
	Data Exit(Captain& cap, State toState) override; // handle when change to other state
	void OnKeyUp(Captain& cap, BYTE keyCode) override;
	void OnKeyDown(Captain& cap, BYTE keyCode) override;
	void Update(Captain& cap, float dt, const std::vector<GameObject*>& coObjects) override; // captain delegates his update call to this function
	void HandleCollisions(Captain& cap, float dt, const std::vector<GameObject*>& coObjects) override;
};

