#pragma once
#include "Item.h"
class FivePointItem :
	public Item
{
public:
	FivePointItem(Vector2 pos, float maxY, State state = State::Invisible);
	virtual void Collect()
	{
		this->SetState(State::Destroyed);
	}// call when captain collect item, play sound and item being destroyed
};

