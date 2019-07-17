#include "pch.h"
#include "FivePointItem.h"

FivePointItem::FivePointItem(Vector2 pos, float maxY, State state) :Item(pos, maxY, state)
{
	animations.emplace(State::Invisible, Animation(SpriteId::Invisible, 0.1f));
	animations.emplace(State::ItemFalling, Animation(SpriteId::ItemFivePointItem, 0.1f));
	animations.emplace(State::ItemNotMoving, Animation(SpriteId::ItemFivePointItem, 0.1f));
}