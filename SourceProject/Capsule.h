#pragma once
#include "VisibleObject.h"

class Capsule :	public VisibleObject
{
private:
	bool isRealItemDropped = false;
	const SpriteId realItemType;
	const float maxY; // maxY of items to spawn
	Grid* grid;

public:
	Capsule(Vector2 pos, SpriteId itemType, float maxY, Grid* grid);
	void Update(float dt, const std::vector<GameObject*>& coObjects = {}) override;
	void BeingHit();
};

