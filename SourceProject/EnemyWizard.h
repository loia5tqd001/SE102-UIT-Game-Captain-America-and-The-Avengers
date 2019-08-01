#pragma once
#include "Enemy.h"

class EnemyWizard : public Enemy
{
private:
	static constexpr float WALKING_SPEED = 80.0f;
	static constexpr float FLYING_SPEED = 150.0f;
	static constexpr float FALLING_SPEED = 100.0f;
	static constexpr float FALL_BACK = 70.0f;
	static constexpr float GROUND = 145.0f;
	static constexpr float ROOF = 15.0f;
	static constexpr float MAX_POS_X = 210.0f;
	static constexpr float MIN_POS_X = 20.0f;
	static constexpr float LIGHT_POS_X = 50.0f;
	static constexpr float LIGHT_POS_Y = 50.0f;
	static constexpr Behaviors ACTIONS_LIST[21] = 
	{ 
	  Behaviors::EnemyWizard_FlyingShoot  , Behaviors::EnemyWizard_GroundShoot  , Behaviors::EnemyWizard_Laught,
	  Behaviors::EnemyWizard_GroundShoot  ,
	  Behaviors::EnemyWizard_TurnOffLight , Behaviors::EnemyWizard_Laught       ,
	  Behaviors::EnemyWizard_FlyBackCorner, Behaviors::EnemyWizard_RunToCap     ,
	  Behaviors::EnemyWizard_GroundShoot  , Behaviors::EnemyWizard_Laught       ,
	  Behaviors::EnemyWizard_Jump         , Behaviors::EnemyWizard_FlyBackCorner,
	  Behaviors::EnemyWizard_FlyingShoot  , Behaviors::EnemyWizard_RunToCap     , Behaviors::EnemyWizard_Jump,
	  Behaviors::EnemyWizard_FlyBackCorner, Behaviors::EnemyWizard_GroundShoot  , Behaviors::EnemyWizard_FlyBackCorner,
	  Behaviors::EnemyWizard_RunToCap     , Behaviors::EnemyWizard_RunToCap     , Behaviors::EnemyWizard_FlyBackCorner
	};

	Captain& cap;
	Behaviors curBehavior = Behaviors::EnemyWizard_FlyingShoot;
	bool Onbehaviors(Behaviors behavior);
	void Action();
	void UpdateAnimation(float dt) override;
public:
	EnemyWizard(Vector2 spawnPos, Vector2 vel, int nx, Grid * grid, Captain& cap);
	void SetState(State state) override;
	void SpawnBullet();
	void Update(float dt, const std::vector<GameObject*>& coObjects) override;
	void SpawnBulletFire();
	void HandleCollisions(float dt, const std::vector<GameObject*>& coObjects);
	void TakeDamage(int damage) override;
	void Render() const override;
	bool isDefeated() { return health <= 0; }
};

