#pragma once
#include "VisibleObject.h"
class ElectricTrap : public VisibleObject
{
private:
	static constexpr float holdTime = 2.5f;
	static constexpr float sleepTime = holdTime;
	static float holdingTime;
	static State syncState;
	int countUpdateTime = 0;
	static float deltaTime;
public:
	ElectricTrap(Vector2 pos);
	void Update(float dt, const std::vector<GameObject*>& coObjects = {}) override;
	RectF GetBBox() const override;
};

