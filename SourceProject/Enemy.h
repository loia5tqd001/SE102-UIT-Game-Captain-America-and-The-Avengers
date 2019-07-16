#pragma once
class Enemy :
	public VisibleObject
{
protected:
	UINT health;
	Grid* grid;
	void OnFlasing();
	virtual void HandleCollisions(float dt, const std::vector<GameObject*>& coObjects)=0;
	virtual void HandleNoCollisions(float dt)=0;

public:
	Enemy(State state, Vector2 spawnPos, Vector2 vel);

	virtual void TakeDamage(UINT damage)=0;
};

