#pragma once

class Enemy : public VisibleObject
{
protected:
	static constexpr float FALL_BACK = 50.0f;

	int health;
	Grid* const grid;
	const State beforeExplode;
	const Behaviors behavior;
	const Data& behaviorData;
	void UpdateAnimation(float dt);

public:
	Enemy(Behaviors behavior, const Data& behaviorData, State beforeExplode, int health, Vector2 spawnPos, Grid* grid);

	void TakeDamage(int damage);
	void OnOutOfViewPort() override { SetState(State::Explode); }
};

