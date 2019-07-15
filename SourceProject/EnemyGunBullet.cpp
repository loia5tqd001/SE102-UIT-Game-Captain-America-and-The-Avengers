#include "pch.h"
#include "EnemyGunBullet.h"
#include "VisibleObject.h"

EnemyGunBullet::EnemyGunBullet(const Vector2 & spawnPos, const Vector2 & vel) : VisibleObject(State::EnemyGunBullet, spawnPos, vel)
{
	animations.emplace(State::EnemyGunBullet, Animation(SpriteId::EnemyGunBullet, 0.1f));
}

RectF EnemyGunBullet::GetBBox() const
{
	return VisibleObject::GetBBox();
}


void EnemyGunBullet::Update(float dt, const std::vector<GameObject*>& coObjects)
{
	if (curState == State::Destroyed) return;

	pos.x += BULLET_MOVING * dt;

	Utils::Clamp(pos.x, 0.0f, (float) Window::Instance().GetWidth(), [this]()
	{
		curState = State::Destroyed;
	});

}

void EnemyGunBullet::Initnx(int nx)
{
	this->nx = nx;
}

