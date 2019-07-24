#pragma once
#include "CaptainState.h"

class CaptainSpinning : public CaptainState
{
private:
	float timeUp = 0;
	float timeDown = 0;
	bool isKicked = false;
	int beginnx = 1;
	float counterTimeFlip = 0;

	bool SetAnotherState = false; //To ignore Stop current update function after set to another state
public:
	void Enter(Captain& cap, State fromState, Data&& data) override;
	Data Exit(Captain& cap, State toState) override;
	void OnKeyUp(Captain& cap, BYTE keyCode) override;
	void OnKeyDown(Captain& cap, BYTE keyCode) override;
	void Update(Captain& cap, float dt, const std::vector<GameObject*>& coObjects) override;
	void HandleCollisions(Captain& cap, float dt, const std::vector<GameObject*>& coObjects) override;
};

