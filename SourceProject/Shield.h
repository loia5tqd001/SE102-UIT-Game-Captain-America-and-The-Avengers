#pragma once

class Shield : public VisibleObject
{ 
private:
	static constexpr float MAX_DISTANCE = 130.0f;
	static constexpr float SPEED = 350.0f;
	bool isOnCaptain = true;
	float distance = 0;
	class Captain& cap;
	void flipPosx();
	float timeToThrow = 0.0f;
	bool isMoved = false;

	//TODO: write this later, after finishing the animation with captain
	void HandleSideCollison(float dt, const std::vector<GameObject*>& coObjects);  //with bullet
	void HandleUpCollison(float dt, const std::vector<GameObject*>& coObjects); //with bullet
	void HandleStraightCollison(float dt, const std::vector<GameObject*>& coObjects); //with bullet
	void HandleBottomCollison(float dt, const std::vector<GameObject*>& coObjects);  //with enemy
	void HandleCaptainCollison(float dt, const std::vector<GameObject*>& coObjects); //with cap
public:
	Shield(Captain& cap);
	void UpdateByCapState(State capState, Vector2 capPos, float dt = -1.0f);
	void Update(float dt, const std::vector<GameObject*>& coObjects = {});
	void PrecheckAABB(float dt, const std::vector<GameObject*>& coObjects = {});
	void ThrowAway();
	void SetVelocity(const Vector2 &vel);
	RectF GetBBox() const override;
	float GetVelX() { return vel.x; }
};


