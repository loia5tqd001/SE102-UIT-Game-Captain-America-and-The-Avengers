#pragma once
#include"Captain.h"

class Shield : public VisibleObject
{
private:
	static constexpr float MAX_DISTANCE = 150.0f;
	static constexpr float SPEED = 260.0f;
	bool isOnCaptain=true;
	float distance = 0;
	Captain *cap;
	void UpdateByCapState(State capState, Vector2 capPos);
	void CalculateVely(float dt);
	void SetState(State state) override;
	void flipPosx();
	//TODO: write this later, after finishing the animation with captain
	void HandleSideCollison(float dt, const std::vector<GameObject*>& coObjects);  //with bullet
	void HandleUpCollison(float dt, const std::vector<GameObject*>& coObjects); //with bullet
	void HandleBottomCollison(float dt, const std::vector<GameObject*>& coObjects);  //with enemy
	void HandleCaptainCollison(float dt, const std::vector<GameObject*>& coObjects); //with cap
public:
	Shield(Captain *captain);
	void Update(float dt, const std::vector<GameObject*>& coObjects = {});
	void ThrowAway();
};

