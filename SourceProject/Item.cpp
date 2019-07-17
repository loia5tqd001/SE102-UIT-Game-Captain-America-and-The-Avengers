#include "pch.h"
#include "Item.h"

Item::Item(Vector2 pos, float maxY, SpriteId itemType) :
	VisibleObject(State::Item_Hide, pos),
	maxPosY(maxY) ,
	minPosY(pos.y - 10.0f),
	itemType(itemType)
{
	shouldDrawImage = false;

	animations.emplace(State::Item_Hide			, Animation(itemType, 0.1f));
	animations.emplace(State::Item_Bouncing		, Animation(itemType, 0.1f));
	animations.emplace(State::Item_Falling		, Animation(itemType, 0.1f));
	animations.emplace(State::Item_Grounding	, Animation(itemType, 0.1f));
	animations.emplace(State::Item_Disappearing	, Animation(itemType, 0.1f));

	switch (itemType)
	{
		case SpriteId::ItemSmallPowerStone:
		case SpriteId::ItemBigPowerStone  :
		case SpriteId::ItemFivePoint      :
		case SpriteId::ItemOneUp          :
			sound = SoundId::ItemNormal;
			break;

		case SpriteId::ItemSmallEnergy    :
		case SpriteId::ItemBigEnergy      :
			sound = SoundId::ItemEnergy;
			break;

		case SpriteId::ItemKeyKrystal     :
			sound = SoundId::ItemKrystal;
			break;

		default:
			ThrowMyException("SpriteId is not item type: ", (int)itemType);
	}
}

void Item::Update(float dt, const std::vector<GameObject*>& coObjects)
{
	switch (curState)
	{
		case State::Item_Hide: // no self update when item is being hide
			return;

		case State::Item_Bouncing:
			pos.y -= FALLING_SPEED * dt; 
			if (pos.y < minPosY)
			{
				pos.y = minPosY;
				curState = State::Item_Falling;
			}
			break;

		case State::Item_Falling:
			pos.y += 100.0f * dt; 
			if (pos.y > maxPosY)
			{
				pos.y = maxPosY;
				curState = State::Item_Grounding;
			}
			break;

		case State::Item_Grounding:
			timeOnGround += dt;
			if (timeOnGround >= TIME_TO_DISAPPEAR)
			{
				curState = State::Item_Disappearing;
				OnFlashing(true);
			}
			break;

		case State::Item_Disappearing:
			if (!isFlashing) // done flashing, time to real disappear
				curState = State::Destroyed;
			break;
	}

	OnFlashing(); // update flashing if it's being set
	animations.at(curState).Update(dt);
}

void Item::BeingHit()
{
	if (curState == State::Item_Hide)
	{
		curState = State::Item_Bouncing;
		shouldDrawImage = true;
	}
}

void Item::BeingCollected()
{
	if (curState == State::Item_Hide) return;
	Sounds::PlayAt(sound);
	curState = State::Destroyed;
}
