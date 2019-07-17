#pragma once
#include"Captain.h"

class Shield : public VisibleObject
{
private:
	State stateAtLastFrame;
	float maximumDistance; 
public:
	Shield(Vector2 capPos);
	void Update(float dt, const Captain& cap, const std::vector<GameObject*>& coObjects = {});
	~Shield();
};


