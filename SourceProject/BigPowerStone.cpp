#include "pch.h"
#include "BigPowerStone.h"

BigPowerStone::BigPowerStone(Vector2 pos, float maxY, State state):Item(pos,maxY,state)
{
	animations.emplace(State::Invisible, Animation(SpriteId::Invisible, 0.1f));
	animations.emplace(State::ItemMoving, Animation(SpriteId::ItemBigPowerStone, 0.1f));
	animations.emplace(State::ItemNotMoving, Animation(SpriteId::ItemBigPowerStone, 0.1f));
}

