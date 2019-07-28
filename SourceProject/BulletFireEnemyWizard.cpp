#include "pch.h"
#include "BulletFireEnemyWizard.h"


BulletFireEnemyWizard::BulletFireEnemyWizard(int nx, const Vector2& spawnPos, bool isVertical, Enemy *enemy) :
	Bullet(State::BulletFireEnemyWizard_Horizontal, 2, spawnPos, { BULLET_MOVING, 0.0f}, nx,enemy)
{
	animations.emplace(State::BulletFireEnemyWizard_Horizontal, Animation(SpriteId::BulletFireEnemyWizard_Horizontal, 0.1f));
	animations.emplace(State::BulletFireEnemyWizard_Vertical, Animation(SpriteId::BulletFireEnemyWizard_Vertical, 0.1f));
	if (isVertical)
	{
		curState = State::BulletFireEnemyWizard_Vertical;
		vel = { 0.0f,BULLET_MOVING };
	}
	if (nx < 0) GameObject::FlipPosXToLeft(pos.x, enemy->GetPosX(), this->GetWidth(), enemy->GetWidth()); // this code is critical
}

void BulletFireEnemyWizard::Update(float dt, const std::vector<GameObject*>& coObjects)
{
	if (curState == State::Destroyed) return;
	pos.y += vel.y * dt;
	pos.x += nx * vel.x * dt;
	animations.at(curState).Update(dt); //do we really need this line?
}

RectF BulletFireEnemyWizard::GetBBox() const
{
	return VisibleObject::GetBBox();
}
