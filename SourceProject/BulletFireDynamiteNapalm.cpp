#include "pch.h"
#include "BulletFireDynamiteNapalm.h"


BulletFireDynamiteNapalm::BulletFireDynamiteNapalm(int nx, Enemy *enemy, const Vector2& spawnPos, Vector2 vel) :
	Bullet(State::DynamiteNapalm_FireBullet, 3, spawnPos, { nx * BULLET_MOVING, 0.0f }, nx, enemy)
{
	animations.emplace(State::DynamiteNapalm_FireBullet, Animation(SpriteId::DynamiteNapalm_FireBullet, 0.1f));
	if (nx < 0) GameObject::FlipPosXToLeft(pos.x, enemy->GetPosX(), this->GetWidth(), enemy->GetWidth());
	Sounds::PlayAt(SoundId::BulletLazer);
}

void BulletFireDynamiteNapalm::Update(float dt, const std::vector<GameObject*>& coObjects)
{
	pos.x += vel.x * dt;
	pos.y += vel.y * dt;
	animations.at(curState).Update(dt);
}

RectF BulletFireDynamiteNapalm::GetBBox() const
{
	return VisibleObject::GetBBox();
}