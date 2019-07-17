#include "pch.h"
#include "Capsule.h"
#include "Item.h"

Capsule::Capsule(Vector2 pos, SpriteId itemType, float maxY, Grid* grid) :
	VisibleObject(State::Capsule_Idle, pos),
	realItemType(itemType),
	maxY(maxY),
	grid(grid)
{
	animations.emplace(State::Capsule_Idle, Animation(SpriteId::CapsuleIdle));
	animations.emplace(State::Capsule_Openning, Animation(SpriteId::CapsuleOpenning, 0.2f));
}

void Capsule::Update(float dt, const std::vector<GameObject*>& coObjects)
{
	if (curState == State::Capsule_Idle) return;

	// else State::Capsule_Openning
	animations.at(curState).Update(dt);

	if (animations.at(curState).IsDoneCycle())
	{
		SetState(State::Capsule_Idle);
	}	
}

void Capsule::BeingHit()
{
	if (curState == State::Capsule_Openning) return;
	else SetState(State::Capsule_Openning);

	if (!realItemDropped) { 
		// TODO: open comment when have grid
		//auto realItem = grid->SpawnObject(std::make_unique<Item>(pos, maxY, realItemType));
		//dynamic_cast<Item*>(realItem)->BeingHit();		
		realItemDropped = true; 
	}
	else {
		//auto fivePointItem = grid->SpawnObject(std::make_unique<Item>(pos, maxY, SpriteId::ItemFivePoint));
		//dynamic_cast<Item*>(fivePointItem)->BeingHit();
	}
}
