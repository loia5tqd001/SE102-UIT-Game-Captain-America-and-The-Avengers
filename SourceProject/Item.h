#pragma once
#include "VisibleObject.h"

class Item : public VisibleObject
{
private:
	float maxPosY; // max position falling, being set from database
	static constexpr float FALLING_SPEED = 150.0f;
public:
	Item(Vector2 pos, float maxY, State state) :
		VisibleObject(state, pos),
		maxPosY(maxY) 
	{
		shouldDrawImage = false;
		// sub class: emplace animation...
	}

	void SetState(State state) override
	{
		switch (state)
		{
		case State::Invisible:
			break;
		case State::Destroyed:
			//Todo: PlaySound
			shouldDrawImage = false;
			break;
		case State::ItemFalling:
		case State::ItemNotMoving:
			shouldDrawImage = true;
			break;
		default:
			break;
		}

		curState = state;
	}

	void Update(float dt, const std::vector<GameObject*>& coObjects) override 
	{
		if (curState==State::Invisible||curState==State::ItemNotMoving)
			return; // item hasn't been hit and appeared yet

		// falling
		if (pos.y < maxPosY)
		{
			pos.y += FALLING_SPEED * dt; 
			//pos.y = min(maxPosY, pos.y); // to not fall too far
		}

		animations.at(curState).Update(dt);
	}

	void Fall() // call when captain hit not claimed item
	{
		this->SetState(State::ItemFalling);
	}

	virtual void Collect() = 0; // call when captain collect item, play sound and item being destroyed
};

