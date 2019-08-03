#pragma once
#include "Enemy.h"

class DynamiteNapalm : public Enemy
{
private:
	static constexpr auto FALLING_SPEED = 250.0f;
	static constexpr auto RUNNING_SPEED = 80.0f;
	static constexpr auto RUNNING_BEFORE_DEAD_SPEED = 1.2f*RUNNING_SPEED;
	static constexpr auto NUM_FIRE_BULLET = 2;
	static constexpr auto DEFAULT_HEALTH = 20;
	static constexpr auto maxHealthHeadless = 8;
	static constexpr auto TIME_TO_HEADLESS_SHOOT = 1.5f;
	static constexpr auto FAR_DISTANCE = 70.0f;
	static constexpr auto INJURE_TIME = 0.6f;

	bool dynamiteThrown = false;
	Captain& cap;
	float posXDesination;
	bool onTheGround = false;
	State beforeFlashingState = State::NotExist;
	Behaviors beforeFlashingBehavior = Behaviors::EnemyFly_Stupid;
	float holdingDistance = 0.0f;
	float maxDistance_IntactRunning = 120.0f;
	int numFireBulletFired = 0;
	float holdTimeHeadlessStanding = TIME_TO_HEADLESS_SHOOT; //Shoot immediately
	float holdTimeInjure;
	bool OnBehavior(Behaviors behavior, float dt);
	bool CanTakeDamage();
public:
	DynamiteNapalm(Behaviors behavior, Data&& behaviorData, Vector2 spawnPos, Vector2 vel, int nx, Grid * grid, Captain& cap);
	void SetState(State state) override;
	void Update(float dt, const std::vector<GameObject*>& coObjects) override;
	void HandleCollisions(float dt, const std::vector<GameObject*>& coObjects);
	void SpawnDynamite();
	void SpawnFireBullet();
	void TakeDamage(int damage) override;
	RectF GetBBox() const override;
	void TakeDinamiteDamage(int damage);
	bool CanCauseElectricShock() { return (curState == State::DynamiteNapalm_Headless_Running_Shooting); }
};

