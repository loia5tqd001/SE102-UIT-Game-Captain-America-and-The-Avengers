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
	animations.emplace(State::Capsule_Openning, Animation(SpriteId::CapsuleOpenning, 0.4f));
	if (itemType == SpriteId::ItemKeyKrystal) bboxColor = Colors::DimRed;
}

void Capsule::Update(float dt, const std::vector<GameObject*>& coObjects)
{
	shouldDrawImage = !SceneManager::Instance().GetCurScene().isDark;

	if (curState == State::Capsule_Idle) return;

	// State::Capsule_Openning
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

	auto itemType = countRealItem > 0 ? realItemType : SpriteId::ItemFivePoint;

	// to make sure item's symmetric
	auto itemWidth = Sprites::Get(itemType).GetFrameSize(0).GetWidth();
	auto dx = ( GetWidth() - float(itemWidth) ) / 2;
	auto posSpawn = pos + Vector2{ dx, 0.0f };

	auto item = grid->SpawnObject(std::make_unique<Item>(posSpawn, maxY, itemType, this));
	dynamic_cast<Item*>(item)->BeingHit();	

	if (itemType != SpriteId::ItemFivePoint)
		countRealItem--;
}
