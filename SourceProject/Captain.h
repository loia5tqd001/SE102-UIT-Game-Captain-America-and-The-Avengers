#pragma once
#include "VisibleObject.h"
#include "CaptainHealth.h"

class Captain : public VisibleObject
{
private:
	static constexpr float WALKING_SPEED = 80.0f;
	static constexpr float JUMPING_SPEED = 500.0f;
	static constexpr float GRAVITY = 1200.0f;

	CaptainHealth& health = CaptainHealth::Instance();
	bool shieldOn = true;
	void ProcessInput();
	void HandleNoCollisions(float dt);
	void HandleCollisions(float dt, const std::vector<GameObject*>& coObjects);
	bool isInTheAir;
	bool isStandingOnTheGround(){return (pos.y > 200);}
	
	//For super speed Captain
	float doubleKeyDownTimeOut;
	//Todo: Use this
	State prevState;
public:
	Captain(const Vector2 & spawnPos);
	Vector2 GetPos() { return pos; };
	int GetNx() { return nx; };
	void OnKeyDown(BYTE keyCode);
	void SetState(State state) override;
	void Update(float dt, const std::vector<GameObject*>& coObjects) override;
	void setShieldOn(bool b) { this->shieldOn = b; } //shield let captain know
};

