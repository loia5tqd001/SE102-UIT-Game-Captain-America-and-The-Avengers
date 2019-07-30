#include "pch.h"
#include "BulletEnemyWizard.h"


BulletEnemyWizard::BulletEnemyWizard(int nx, const Vector2& spawnPos, Vector2& vel,Enemy *enemy) :
	Bullet(State::BulletEnemyWizard_Horizontal, 4, spawnPos, vel, nx, enemy)
{
	animations.emplace(State::BulletEnemyWizard_Cross, Animation(SpriteId::BulletEnemyWizard_Cross, 0.1f));
	animations.emplace(State::BulletEnemyWizard_Horizontal, Animation(SpriteId::BulletEnemyWizard_Horizontal, 0.1f));
	animations.emplace(State::BulletEnemyWizard_SemiCross, Animation(SpriteId::BulletEnemyWizard_SemiCross, 0.1f));

	if (nx < 0) GameObject::FlipPosXToLeft(pos.x, enemy->GetPosX(), this->GetWidth(), enemy->GetWidth()); // this code is critical
}

void BulletEnemyWizard::Update(float dt, const std::vector<GameObject*>& coObjects)
{
	if (curState == State::Destroyed) return;
	if (abs(vel.y) > abs(vel.x)/1.5) curState = State::BulletEnemyWizard_Cross;
	else if (abs(vel.y) > abs(vel.x)/2.5) curState = State::BulletEnemyWizard_SemiCross;
	else curState = State::BulletEnemyWizard_Horizontal;
	pos.y += vel.y * dt;
	pos.x += vel.x * dt;
	animations.at(curState).Update(dt);
}

RectF BulletEnemyWizard::GetBBox() const
{
	return VisibleObject::GetBBox();
}

void BulletEnemyWizard::Reflect()
{
	vel.x = -vel.x/sqrt(2);
	vel.y = std::abs(vel.x);
}
