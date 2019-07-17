#include "pch.h"
#include "SmallPowerStone.h"


SmallPowerStone::SmallPowerStone(Vector2 pos, float maxY, State state):Item(pos,maxY,state)
{
	animations.emplace(State::Invisible, Animation(SpriteId::Invisible, 0.1f));
	animations.emplace(State::ItemMoving, Animation(SpriteId::ItemSmallPowerStone, 0.1f));
	animations.emplace(State::ItemNotMoving, Animation(SpriteId::ItemSmallPowerStone, 0.1f));
}