#pragma once

class Spawner : public InvisibleObject
{
private:
	const Behaviors objectBehavior;
	const Vector2 objSpawnPos;
	//const Data behaviorData;
	const int expectCapNx;
	Grid* const grid;
	std::shared_ptr<class Enemy> enemy = nullptr;
	bool readyToSpawn = true;
	bool isActive = true; // control by ambush trigger to stall spawner to spawn ambush soldiers

public:
	Spawner(Vector2 pos, UINT w, UINT h, Behaviors behav, Vector2 objSpawnPos, int expectCapNx, Data&& data, Grid* grid);
	void Update(float dt, const std::vector<GameObject*>& coObjects = {}) override;
	void OnCollideWithCap(class Captain* cap); // call when captain collide with spawners
	void SetActive(bool isactive) { isActive = isactive; } // set by ambush trigger
};

