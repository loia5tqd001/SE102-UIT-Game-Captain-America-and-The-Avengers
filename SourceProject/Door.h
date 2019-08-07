#pragma once
#include "VisibleObject.h"
#include "PittsburghScene.h"

class Door : public VisibleObject
{
private:
	bool isDark;

public:
	Door(Vector2 pos);
	void Update(float dt, const std::vector<GameObject*>& coObjects = {}) override;
	void Open();
	RectF GetBBox() const;
};

