#pragma once

class Enemy : public VisibleObject
{
protected:
	static constexpr float FALL_BACK = 100.0f;

	int health;
	Grid* grid;
	State beforeExplode;
    State Explode = State::Explode; //wizard need this
    Behaviors behavior;
	const Data& behaviorData;

	void UpdateAnimation(float dt);
public:
	Enemy(Behaviors behavior, const Data& behaviorData, State beforeExplode, int health, Vector2 spawnPos, Grid* grid, State initState = State::NotExist);
	//enemy fly dont need data, just put pos to it and it will be fine
	//also not every enemy will explode

	void HandleCollisions(float dt, const std::vector<GameObject*>& coObjects);
	RectF GetBBox() const override;
	virtual void TakeDamage(int damage);
	float GetPosX() { return pos.x; }
	bool IsInViewPort() const { return GetBBox().IsIntersect(Camera::Instance().GetBBox()); }
};

