#pragma once
#include "CaptainState.h"

class CaptainJumping : public CaptainState
{
	static constexpr auto JUMP_SPEED_VER = 100.0f;
	static constexpr auto JUMP_SPEED_HOR = 100.0f;
private:
	bool isJumpReleased;
	bool canHigher;

public:
	void Enter(Captain& cap, State fromState, Data&& data) override;
	Data Exit(Captain& cap, State toState) override;
	void OnKeyUp(Captain& cap, BYTE keyCode) override;
	void OnKeyDown(Captain& cap, BYTE keyCode) override;
	void Update(Captain& cap, float dt, const std::vector<GameObject*>& coObjects) override;
	void HandleCollisions(Captain& cap, float dt, const std::vector<GameObject*>& coObjects) override;

};

