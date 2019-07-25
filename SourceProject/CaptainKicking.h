#pragma once
#include "CaptainState.h"

class CaptainKicking : public CaptainState
{
private:
	State lastState;
	bool isJumpReleased;
	float JumpHeightRealCounter;
	float JumpHeightNeedCounter;
	float timeUp = 0;
	float timeDown = 0;
	bool isKicked = true;

	bool SetAnotherState = false; //To end Update after Handle collision
	
	bool firstTimeUpdate = true;
public:
	void Enter(Captain& cap, State fromState, Data&& data) override;
	Data Exit(Captain& cap, State toState) override;
	void OnKeyUp(Captain& cap, BYTE keyCode) override;
	void OnKeyDown(Captain& cap, BYTE keyCode) override;
	void Update(Captain& cap, float dt, const std::vector<GameObject*>& coObjects) override;
	void HandleCollisions(Captain& cap, float dt, const std::vector<GameObject*>& coObjects) override;
};

