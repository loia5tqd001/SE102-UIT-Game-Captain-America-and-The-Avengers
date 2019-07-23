#pragma once
#include "CaptainState.h"

class CaptainSpinning : public CaptainState
{
private:
	static constexpr auto SPIN_SPEED_VER = 50.0f;
	static constexpr auto SPIN_SPEED_HOR = 50.0f;
	static constexpr auto TIME_KEEP_SPIN = 0.3f;

	float timeUp = 0;
	float timeDown = 0;
	bool isKicked = false;
public:
	void Enter(Captain& cap, State fromState, Data&& data) override;
	Data Exit(Captain& cap, State toState) override;
	void OnKeyUp(Captain& cap, BYTE keyCode) override;
	void OnKeyDown(Captain& cap, BYTE keyCode) override;
	void Update(Captain& cap, float dt, const std::vector<GameObject*>& coObjects) override;
	void HandleCollisions(Captain& cap, float dt, const std::vector<GameObject*>& coObjects) override;
};

