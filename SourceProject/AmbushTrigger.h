#pragma once

class AmbushTrigger : public InvisibleObject
{
	State state = State::Ambush_NotYet;
	Captain* captain;
	Grid* const grid; // to spawn enemies
	const RectF lockCamera; // to lock camera
	const RectF lockCaptain;  // to lock captain
	std::vector<class Spawner*> spawners; // to reactive spawners when ambush is gone
	const Vector2 enemyGunSpawnPos; // to know where to spawn enemygun
	const Vector2 enemyRocketSpawnPos; // to know where to spawn enemyrocket

	std::shared_ptr<class EnemyGun> enemyGun;
	std::shared_ptr<class EnemyRocket> enemyRocket;
	int countEnemyGun = 3, countEnemyRocket = 3;

	void SpawnEnemyGun();
	void SpawnEnemyRocket();

	static AmbushTrigger* instance;

public:
	AmbushTrigger(Vector2 pos, UINT w, UINT h, const RectF& lockRegion, Grid* grid);

	const RectF& GetLockCamera() const;
	const RectF& GetLockCaptain() const;
	inline State GetState() const override { return state; } 

	void Update(float dt, const std::vector<GameObject*>& coObjects = {}) override;
	void OnCollideWithCap(Captain* cap); // call when captain first collide with ambush trigger

	static void SetInstance(AmbushTrigger* inst);	
	static AmbushTrigger*& Instance();
};

