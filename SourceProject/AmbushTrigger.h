#pragma once

class AmbushTrigger : public InvisibleObject
{
	bool isActive = false;
	bool isDestroyed = false;
	const RectF lockRegion; 
	Grid* const grid; // to spawn enemy
	std::vector<class Spawner*> spawners;
	Vector2 enemyGunSpawnPos;
	Vector2 enemyRocketSpawnPos;

public:
	AmbushTrigger(Vector2 pos, UINT w, UINT h, const RectF& lockRegion, Grid* grid);

	// clamp camera inside lock region
	// clamp captain inside lock region .Trim(20, 0, 20, 0)
	inline const RectF& GetLockRegion() const { return lockRegion; }
	inline bool IsActive() const { return isActive; } // check if ambush trigger is active, if yes, spawners should be stalling
	void Active(); // call when captain first collide with ambush trigger

	State GetState() const override;
	void Update(float dt, const std::vector<GameObject*>& coObjects = {}) override;
	

};

