#pragma once
#include "CaptainState.h"

class CaptainCoverLow : public CaptainState
{
	static constexpr auto GRAVITY = 170.0f;
	static constexpr auto MOVE_HOR = 50.0f;

	bool isOnGround = false;
public:
	void Enter(Captain& cap, State fromState, Data&& data) override;
	Data Exit(Captain& cap, State toState) override;
	void OnKeyUp(Captain& cap, BYTE keyCode) override;
	void OnKeyDown(Captain& cap, BYTE keyCode) override;
	void Update(Captain& cap, float dt, const std::vector<GameObject*>& coObjects) override;
	void HandleCollisions(Captain& cap, float dt, const std::vector<GameObject*>& coObjects) override;
};

