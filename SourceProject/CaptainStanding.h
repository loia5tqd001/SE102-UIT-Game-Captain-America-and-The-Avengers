#pragma once
#include "CaptainState.h"

class CaptainStanding : public CaptainState
{
	static constexpr auto WALKING_SPEED = 150.0f;
private:
	bool isToSittingTackle = false;
	bool isOnGround = false;
	bool isOnMovingLedge = false;
	GameObject* movingLedge = nullptr;
public:
	void SetOnMovingLedge(bool is_on_moving_ledge, GameObject* movingLedge = nullptr);
	void Enter(Captain& cap, State fromState, Data&& data) override;
	Data Exit(Captain& cap, State toState) override;
	void OnKeyUp(Captain& cap, BYTE keyCode) override;
	void OnKeyDown(Captain& cap, BYTE keyCode) override;
	void Update(Captain& cap, float dt, const std::vector<GameObject*>& coObjects) override;
	void HandleCollisions(Captain& cap, float dt, const std::vector<GameObject*>& coObjects) override;
};


