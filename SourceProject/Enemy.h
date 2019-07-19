#pragma once

class Enemy : public VisibleObject
{
protected:
	static constexpr float FALL_BACK = 50.0f;

	int health;
	Grid* const grid;
	State beforeExplode = State::Destroyed;
    State Explode = State::Destroyed; //wizard need to change this
	const Behaviors behavior;
	const Data& behaviorData;

	void UpdateAnimation(float dt);
public:
	Enemy(Behaviors behavior, const Data& behaviorData, State beforeExplode, int health, Vector2 spawnPos, Grid* grid);

	void TakeDamage(int damage);
	void OnOutOfViewPort() override { SetState(State::Explode); }
	float GetPosX() { return pos.x; }
};

