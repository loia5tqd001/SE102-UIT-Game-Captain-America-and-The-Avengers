#pragma once
#include "VisibleObject.h"
class EnemyGun :
	public VisibleObject
{
public:
	EnemyGun(const Vector2& spawnPos, const Vector2& vel);

	RectF GetBBox() const override;
	void SetState(State state) override;

	UINT GetHeight() const;
	void OnKneeHeight(UINT oldHeight); // recalculate position when lowing down body (height changed)

	void Update(float dt, const std::vector<GameObject*>& coObjects) override;
};

