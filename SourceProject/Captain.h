#pragma once

class Captain : public VisibleObject
{
private:
	friend class CaptainStanding         ; CaptainStanding    stateStanding       ;      
	friend class CaptainWalking          ; CaptainWalking     stateWalking        ;      
	friend class CaptainJumping          ; CaptainJumping     stateJumping        ;    
	friend class CaptainFalling          ; CaptainFalling     stateFalling        ;
	friend class CaptainKicking          ; CaptainKicking     stateKicking        ;      
	friend class CaptainSpinning         ; CaptainSpinning    stateSpinning       ;      
	friend class CaptainThrowing         ; CaptainThrowing    stateThrowing       ;      
	friend class CaptainTackle           ; CaptainTackle      stateTackle         ;      
	friend class CaptainPunching         ; CaptainPunching    statePunching       ;      
	friend class CaptainSitting          ; CaptainSitting     stateSitting        ;      
	friend class CaptainSitPunching      ; CaptainSitPunching stateSitPunching    ;      
	friend class CaptainCoverTop         ; CaptainCoverTop    stateCoverTop       ;      
	friend class CaptainCoverLow         ; CaptainCoverLow    stateCoverLow       ;      
	friend class CaptainFallToWater      ; CaptainFallToWater stateFallToWater    ;      
	friend class CaptainInWater          ; CaptainInWater     stateInWater        ;      
	friend class CaptainSwimming         ; CaptainSwimming    stateSwimming       ;      
	friend class CaptainClimbing         ; CaptainClimbing    stateClimbing       ;      
	friend class CaptainInjured          ; CaptainInjured     stateInjured        ;      
	friend class CaptainDead             ; CaptainDead        stateDead           ;     
	friend class CaptainElectricShock    ; CaptainElectricShock stateElectricShock;
	CaptainState* currentState;
	Grid* grid;

	bool canPhaseThroughFloor;
	State prePhasingState;
	State phasingState;
	bool shieldOn = true;
	bool ignoreUpdate = false;
	Vector2 posBeforePhasing = Vector2{ -1,-1 };
	std::unique_ptr<Shield> shield;
	CaptainHealth& health = CaptainHealth::Instance();
	KeyControls lastKeyDown, lastKeyUp;
	std::chrono::steady_clock::time_point timeLastKeyDown, timeLastKeyUp;

	void PrecheckAABB(const std::vector<GameObject*>& coObjects, float dt); // return true if there's AABB collision with enemy and bullet already
	void CollideWithPassableObjects(float dt, const CollisionEvent& e);
	void HandleHitBox(float dt,  const std::vector<GameObject*>& coObjects);

public:
	Captain(const Vector2& pos,Grid* grid); // captain position is being set when scene is set, no need to use constructor
	void OnKeyDown(BYTE keyCode);
	void OnKeyUp(BYTE keyCode);
	void SetState(State state) override;
	void Update(float dt, const std::vector<GameObject*>& coObjects) override;
	void setShieldOn(bool b) { this->shieldOn = b; } //shield let captain know
	bool IsShieldOn() { return shieldOn; }
	Vector2 GetCenter() const;
	void Render() const override;
	RectF GetBBox() const override;
	RectF GetHitBox() const override;
	bool isDead() { return health.Get() <= 0; };
	int getDirectionWhenThrow() { if (curState == State::Captain_Throwing) return nx; else return 0; }
};

