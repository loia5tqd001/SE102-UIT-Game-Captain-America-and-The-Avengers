#pragma once
#include "VisibleObject.h"
#include "CaptainHealth.h"

class Shield;

class Captain : public VisibleObject
{
private:
	static constexpr float WALKING_SPEED = 60.0f;
	static constexpr float SMASH_SPEED = 400.0f;
	static constexpr float JUMPING_SPEED = 250.0f; //Also be falling speed
	static constexpr float DOUBLE_KEY_DOWN_TIME_OUT = 0.2f;
	static constexpr float MAX_HEIGHT_JUMP = 80.0f;
	static constexpr float MAX_HEIGHT_SPIN = 40.0f;

	std::unique_ptr<Shield> shield;

	CaptainHealth& health = CaptainHealth::Instance();
	bool shieldOn = true;
	void ProcessInput();
	void HandleNoCollisions(float dt);
	void HandleCollisions(float dt, const std::vector<GameObject*>& coObjects);

	bool isInTheAir;

	bool isStandingOnTheGround() { return curState == State::Captain_Standing; }
	Vector2 posWhenJump;

	//For super speed Captain
	KeyControls prevPressedControlKey;
	std::chrono::steady_clock::time_point timePressed;

	//Todo: Use this
	bool canSpin;
	State prevState;
public:
	Captain(const Vector2 & spawnPos);
	Vector2 GetPos() { return pos; };
	int GetNx() { return nx; };
	void OnKeyDown(BYTE keyCode);
	void OnKeyUp(BYTE keyCode);
	void SetState(State state) override;
	State GetState() { return curState; }
	void Update(float dt, const std::vector<GameObject*>& coObjects) override;
	void setShieldOn(bool b) { this->shieldOn = b; } //shield let captain know
	void Render() const override;
};

