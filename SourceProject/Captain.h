#pragma once

class Captain : public VisibleObject
{
private:
	//static constexpr float WALKING_SPEED = 80.0f;
	//static constexpr float SMASH_SPEED = 400.0f;
	//static constexpr float JUMPING_SPEED = 500.0f;
	//static constexpr float GRAVITY = 1200.0f;
	//static constexpr float DOUBLE_KEY_DOWN_TIME_OUT = 0.2f;
	std::unique_ptr<Shield> shield;
	CaptainHealth& health = CaptainHealth::Instance();

	friend class CaptainStanding    ; CaptainStanding    stateStanding    ;      
	friend class CaptainWalking     ; CaptainWalking     stateWalking     ;      
	friend class CaptainJumping     ; CaptainJumping     stateJumping     ;      
	friend class CaptainKicking     ; CaptainKicking     stateKicking     ;      
	friend class CaptainSpinning    ; CaptainSpinning    stateSpinning    ;      
	friend class CaptainThrowing    ; CaptainThrowing    stateThrowing    ;      
	friend class CaptainTackle      ; CaptainTackle      stateTackle      ;      
	friend class CaptainPunching    ; CaptainPunching    statePunching    ;      
	friend class CaptainSitting     ; CaptainSitting     stateSitting     ;      
	friend class CaptainSitPunching ; CaptainSitPunching stateSitPunching ;      
	friend class CaptainCoverTop    ; CaptainCoverTop    stateCoverTop    ;      
	friend class CaptainCoverLow    ; CaptainCoverLow    stateCoverLow    ;      
	friend class CaptainFallToWater ; CaptainFallToWater stateFallToWater ;      
	friend class CaptainInWater     ; CaptainInWater     stateInWater     ;      
	friend class CaptainSwimming    ; CaptainSwimming    stateSwimming    ;      
	friend class CaptainClimbing    ; CaptainClimbing    stateClimbing    ;      
	friend class CaptainInjured     ; CaptainInjured     stateInjured     ;      
	friend class CaptainDead        ; CaptainDead        stateDead        ;      
	CaptainState* currentState;

	bool shieldOn = true;
	//void ProcessInput();
	void CollideWithPassableObjects(float dt, const CollisionEvent& e);
	//void HandleNoCollisions(float dt);
	//void HandleCollisions(float dt, const std::vector<GameObject*>& coObjects);
	//bool isInTheAir;

	//friend class CaptainStanding;
	//std::unique_ptr<CaptainStanding> standing;
	//
	//For super speed Captain
	KeyControls lastKeyDown, lastKeyUp;
	std::chrono::steady_clock::time_point timeLastKeyDown, timeLastKeyUp;

	//Todo: Use this
	bool canSpin;
	State prevState;
public:
	Captain(const Vector2& pos); // captain position is being set when scene is set, no need to use constructor
	void OnKeyDown(BYTE keyCode);
	void OnKeyUp(BYTE keyCode);
	void SetState(State state) override;
	void Update(float dt, const std::vector<GameObject*>& coObjects) override;
	void setShieldOn(bool b) { this->shieldOn = b; } //shield let captain know
	void Render() const override;
	RectF GetBBox() const override;

};

