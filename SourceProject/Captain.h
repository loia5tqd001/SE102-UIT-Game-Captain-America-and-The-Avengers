#pragma once

class Captain : public VisibleObject
{
private:
	friend class CaptainStanding    ; CaptainStanding    stateStanding    ;      
	friend class CaptainWalking     ; CaptainWalking     stateWalking     ;      
	friend class CaptainJumping     ; CaptainJumping     stateJumping     ;    
	friend class CaptainFalling     ; CaptainFalling     stateFalling     ;
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
	std::unique_ptr<Shield> shield;
	CaptainHealth& health = CaptainHealth::Instance();
	KeyControls lastKeyDown, lastKeyUp;
	std::chrono::steady_clock::time_point timeLastKeyDown, timeLastKeyUp;

	void CollideWithPassableObjects(float dt, const CollisionEvent& e);

public:
	Captain(const Vector2& pos); // captain position is being set when scene is set, no need to use constructor
	void OnKeyDown(BYTE keyCode);
	void OnKeyUp(BYTE keyCode);
	void SetState(State state) override;
	void Update(float dt, const std::vector<GameObject*>& coObjects) override;
	void setShieldOn(bool b) { this->shieldOn = b; } //shield let captain know
	Vector2 GetCenter() const;
	void Render() const override;
	RectF GetBBox() const override;
	RectF GetHitBox() const override;
};

