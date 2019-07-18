#pragma once
#include "VisibleObject.h"

class Item : public VisibleObject
{
private:
	static constexpr float FALLING_SPEED = 150.0f;
	static constexpr float TIME_TO_DISAPPEAR = 5.0f;

	float maxY; // max position falling, being set from database
	float minY; // to bounce before falling
	float timeOnGround = 0.0f;
	SoundId sound;
	SpriteId itemType; // use SpriteId to represent item type

public:
	Item(Vector2 pos, float maxY, SpriteId itemType);
	void Update(float dt, const std::vector<GameObject*>& coObjects = {}) override;
	void BeingHit(); // call when captain use force to collide with capsule
	void BeingCollected(); // depend on GetItemType() captain will get corresponding result, item itself won't acknowledge about that result
	auto GetItemType() const { return itemType; }
};

