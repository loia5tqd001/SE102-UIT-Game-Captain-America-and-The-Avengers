#include "pch.h"
#include "Enemy.h"

Enemy::Enemy(State state, Vector2 spawnPos, Vector2 vel) : VisibleObject(state, spawnPos, vel)
{
}
void Enemy::OnFlasing()
{
	static UINT  nFrameUnrendered = 0;
	if (++nFrameUnrendered >= 10) {
		shouldDrawImage = true;
		nFrameUnrendered = 0;
	}
	else {
		shouldDrawImage = false;
	}
}


