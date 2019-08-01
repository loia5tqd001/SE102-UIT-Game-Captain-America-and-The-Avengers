#pragma once
#include "Bullet.h"

class BulletBunker : public Bullet
{
public:
	BulletBunker(Vector2 spawnPos, Vector2 vel);

	void Update(float dt, const std::vector<GameObject*>& coObjects) override;
	void HitCaptain() override;
	void Reflect();
};

