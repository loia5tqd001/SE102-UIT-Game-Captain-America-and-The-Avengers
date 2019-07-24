#pragma once
#include "CaptainState.h"

class CaptainWalking : public CaptainState
{
	static constexpr auto WALKING_SPEED = 65.0f;
private:
	bool isToSittingTackle = false;
	bool isOnGround;
public:
	void Enter(Captain& cap, State fromState, Data&& data) override;
	Data Exit(Captain& cap, State toState) override;
	void OnKeyUp(Captain& cap, BYTE keyCode) override;
	void OnKeyDown(Captain& cap, BYTE keyCode) override;
	void Update(Captain& cap, float dt, const std::vector<GameObject*>& coObjects) override;
	void HandleCollisions(Captain& cap, float dt, const std::vector<GameObject*>& coObjects) override;
};

