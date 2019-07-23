#pragma once

class Spawner : public InvisibleObject
{
private:
	const Behaviors objectBehavior;
	const Vector2 objSpawnPos;
	const Data behaviorData;
	Grid* const grid;
	class Captain* cap; // TODO: Rewrite spawner abit later
	std::shared_ptr<class Enemy> enemy;
	bool readyToSpawn = true;

public:
	Spawner(Vector2 pos, UINT w, UINT h, Behaviors behav, Vector2 objSpawnPos, Data&& data, Grid* grid);
	void Update(float dt, const std::vector<GameObject*>& coObjects = {}) override;
	void OnCollideWithCap(); // call when captain collide with spawners
	void SetActive(bool isactive) { readyToSpawn = isactive; } // set by ambush trigger
};

