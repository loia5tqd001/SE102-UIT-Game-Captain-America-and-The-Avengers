#pragma once
#include "MovingLedge.h"

class MovingLedgeUpdater : public InvisibleObject
{
	const Vector2 DEFAULT_POS_CIRCLE = Vector2{ 632.0f, 640.0f };
	const Vector2 DEFAULT_POS_DIAGONAL = Vector2{ 680.0f, 496.0f };
	const Vector2 DEFAULT_POS_HORIZONTAL = Vector2{ 696.0f, 423.0f };

	std::shared_ptr<MovingLedge> circle, diagonal, horizontal;
public:
	MovingLedgeUpdater(Vector2 pos, UINT w, UINT h, Grid* grid);
	void Update(float dt, const std::vector<GameObject*>& coObjects = {}) override;
	void OnOutOfViewPort() override;
};

