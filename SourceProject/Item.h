#pragma once
#include "VisibleObject.h"

class Item : public VisibleObject
{
private:
	float maxPosY; // max position falling, being set from database
	
public:
	Item(State state, Vector2 pos, float maxY) : 
		VisibleObject(state, pos),
		maxPosY(maxY) 
	{
		shouldDrawImage = false;
		// sub class: emplace animation...
	}

	void Update(float dt, const std::vector<GameObject*>& coObjects) override 
	{
		if (!shouldDrawImage) return; // item hasn't been hit and appeared yet

		// falling
		if (pos.y < maxPosY)
		{
			pos.y += 100.0f * dt; 
			pos.y = max(maxPosY, pos.y); // to not fall too far
		}

		animations.at(curState).Update(dt);
	}

	void Fall() // call when captain hit the item
	{
		shouldDrawImage = true;
	}

	virtual void Collect() = 0; // call when captain collect item, play sound and item being destroyed
};

