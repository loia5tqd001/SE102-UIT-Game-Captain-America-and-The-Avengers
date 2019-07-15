#include "pch.h"
#include "EnemyGun.h"


EnemyGun::EnemyGun(const Vector2 & spawnPos, const Vector2 & vel) :
	VisibleObject(State::EnemyGun_Stand, spawnPos, vel)
{
	animations.emplace(State::EnemyGun_Stand, Animation(SpriteId::EnemyGun_Stand, 0.1f));
	animations.emplace(State::EnemyGun_OnKnee, Animation(SpriteId::EnemyGun_OnKnee, 0.1f));
	animations.emplace(State::EnemyGun_TakeDamage, Animation(SpriteId::EnemyGun_TakeDamage, 0.1f));
	animations.emplace(State::EnemyGun_Walking, Animation(SpriteId::EnemyGun_Walking, 0.1f));
}

void EnemyGun::Update(float dt, const std::vector<GameObject*>& coObjects)
{
	if (curState == State::EnemyGun_TakeDamage || curState == State::Destroyed) return;

	pos.x += vel.x * dt;
	pos.y += vel.y * dt;
	//pos.Neutralize(); // HACK: fix flickering, but will cause weird behaviors in cases. NOTE: Don't call this when vsync is off, will cause lag

	Utils::Clamp(pos.x, 0.0f, 624.0f, [this]() {vel.x = -vel.x; });

	animations.at(curState).Update(dt);
}
void EnemyGun::SetState(State state)
{
	VisibleObject::SetState(state);

	switch (state)
	{
	case State::EnemyGun_Stand:
		vel.x = 0.0f;
		break;
	case State::EnemyGun_OnKnee:
		vel.x = 0.0f;
		break;
	case State::EnemyGun_TakeDamage:
		vel.x = 0.0f;
		break;
	}
}
void EnemyGun::OnKneeHeight(UINT oldHeight)
{
	assert(oldHeight > GetHeight());
	pos.y += oldHeight - GetHeight();
}
UINT EnemyGun::GetHeight() const
{
	return animations.at(curState).GetFrameSize().GetHeight();
}
RectF EnemyGun::GetBBox() const
{
	if (curState == State::EnemyGun_TakeDamage) return {};
	return VisibleObject::GetBBox();
}

