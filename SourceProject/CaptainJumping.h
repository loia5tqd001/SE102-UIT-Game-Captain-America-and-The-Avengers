#pragma once
#include "CaptainState.h"

class CaptainJumping : public CaptainState
{
private:
	bool isJumpReleased = false;
	float JumpHeightRealCounter = 0;
	float JumpHeightNeedCounter = MIN_JUMP_HEIGHT;
	bool isKicked = false;

	bool setAnotherState = false;
	bool ignoreUpdate = false;
	float JumpSpeed = JUMP_SPEED_VER_MAX;

public:
	void Enter(Captain& cap, State fromState, Data&& data) override;
	Data Exit(Captain& cap, State toState) override;
	void OnKeyUp(Captain& cap, BYTE keyCode) override;
	void OnKeyDown(Captain& cap, BYTE keyCode) override;
	void Update(Captain& cap, float dt, const std::vector<GameObject*>& coObjects) override;
	void HandleCollisions(Captain& cap, float dt, const std::vector<GameObject*>& coObjects) override;

};

