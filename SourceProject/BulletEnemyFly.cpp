#include "pch.h"
#include "BulletEnemyFly.h"

BulletEnemyFly::BulletEnemyFly(int nx, Enemy * enemy, const Vector2 & spawnPos, Captain *cap)
	: Bullet(State::BulletEnemyFly_Hor, 1, spawnPos, { 0,0 }, nx, enemy)
{
	animations.emplace(State::BulletEnemyFly_Hor, Animation(SpriteId::BulletEnemyFly_Hor, 0.05f));
	animations.emplace(State::BulletEnemyFly_CrossDown, Animation(SpriteId::BulletEnemyFly_CrossDown, 0.05f));
	animations.emplace(State::BulletEnemyFly_CrossUp, Animation(SpriteId::BulletEnemyFly_CrossUp, 0.05f));
	animations.emplace(State::BulletEnemyFly_Up, Animation(SpriteId::BulletEnemyFly_Up, 0.05f));
	animations.emplace(State::BulletEnemyFly_Down, Animation(SpriteId::BulletEnemyFly_Down, 0.05f));
	animations.emplace(State::Explode, Animation(SpriteId::Explode, 0.1f));

	if (nx < 0) GameObject::FlipPosXToLeft(pos.x, enemy->GetPosX(), this->GetWidth(), enemy->GetWidth()); // this code is critical
	this->cap = cap;
	vel = { BULLET_MOVING * nx, 0.0f };
}

void BulletEnemyFly::Update(float dt, const std::vector<GameObject*>& coObjects)
{
	if (animations.at(State::Explode).IsDoneCycle())
	{
		curState = State::Destroyed;
		return;
	}
	counterExplode += GameTimer::Dt();
	if (counterExplode >= MAX_TIME_TILL_EXPLODE) {
		curState = State::Explode;
		vel = { 0.0f , 0.0f };
	}

	animations.at(curState).Update(dt);
	SetAnimationByVel();
	CalculateVelByCapPos(cap->GetPos().x + cap->GetWidth()/2, cap->GetPos().y + cap->GetHeight()/2);

	pos.x += vel.x * dt;
	pos.y += vel.y * dt;
}

void BulletEnemyFly::SetAnimationByVel()
{
	if (curState == State::Explode || curState == State::Destroyed) return;
	float x = vel.x;
	float y = vel.y;
	if (x >= 0 && y >= 0) {
		nx = 1;
		if (std::abs(x) > std::abs(y) / 2 && std::abs(x) < std::abs(y) * 2)
		{
			curState = (State::BulletEnemyFly_CrossDown);
		}
		else if (std::abs(x) > std::abs(y) * 2)
		{
			curState = (State::BulletEnemyFly_Hor);
		}
		else
		{
			curState = (State::BulletEnemyFly_Down);
		}
	}
	else if (x < 0 && y >= 0){
		nx = -1;
		if (std::abs(x) > std::abs(y) / 2 && std::abs(x) < std::abs(y) * 2)
		{
			curState = (State::BulletEnemyFly_CrossDown);
		}
		else if (std::abs(x) > std::abs(y) * 2)
		{
			curState = (State::BulletEnemyFly_Hor);
		}
		else
		{
			curState = (State::BulletEnemyFly_Down);
		}
	}
	else if (x >= 0 && y < 0) {
		nx = 1;
		if (std::abs(x) > std::abs(y) / 2 && std::abs(x) < std::abs(y) * 2)
		{
			curState = (State::BulletEnemyFly_CrossUp);
		}
		else if (std::abs(x) > std::abs(y) * 2)
		{
			curState = (State::BulletEnemyFly_Hor);
		}
		else
		{
			curState = (State::BulletEnemyFly_Up);
		}
	}
	else if (x < 0 && y < 0) {
		nx = -1;
		if (std::abs(x) > std::abs(y) / 2 && std::abs(x) < std::abs(y) * 2)
		{
			curState = (State::BulletEnemyFly_CrossUp);
		}
		else if (std::abs(x) > std::abs(y) * 2)
		{
			curState = (State::BulletEnemyFly_Hor);
		}
		else
		{
			curState = (State::BulletEnemyFly_Up);
		}
	}
}

void BulletEnemyFly::CalculateVelByCapPos(float x, float y)
{
	if (curState == State::Explode || curState == State::Destroyed) return;
	float dx = x - pos.x;
	float dy = y - pos.y;

	if (dx < 0 && dy < 0) {
		if (vel.x < 0 && vel.y < 0)
		{
			if (abs(dx)/abs(dy) < abs(vel.x) / abs(vel.y))
			{
				vel.x -= MIN_CHANGE_VELX_PER_FRAME;
			}
			else
			{
				vel.x += MIN_CHANGE_VELX_PER_FRAME;
			}
		}
		else if (vel.x >= 0 && vel.y < 0) {
			vel.x += MIN_CHANGE_VELX_PER_FRAME;
		}
		else if (vel.x < 0 && vel.y >= 0) {
			vel.x += MIN_CHANGE_VELX_PER_FRAME;
		}
		else if (vel.x >= 0 && vel.y >= 0) {
			curState = State::Explode;
			vel = { 0.0f, 0.0f };
			return;
		}
	}
	else if (dx >= 0 && dy < 0) {
		if (vel.x < 0 && vel.y < 0)
		{
			vel.x += MIN_CHANGE_VELX_PER_FRAME;
		}
		else if (vel.x >= 0 && vel.y < 0) {
			if (abs(dx) / abs(dy) < abs(vel.x) / abs(vel.y))
			{
				vel.x -= MIN_CHANGE_VELX_PER_FRAME;
			}
			else
			{
				vel.x += MIN_CHANGE_VELX_PER_FRAME;
			}
		}
		else if (vel.x < 0 && vel.y >= 0) {
			curState = State::Explode;
			vel = { 0.0f, 0.0f };
			return;
		}
		else if (vel.x >= 0 && vel.y >= 0) {
			vel.x += MIN_CHANGE_VELX_PER_FRAME;
		}
	}
	else if (dx < 0 && dy >= 0) {
		if (vel.x < 0 && vel.y < 0)
		{
			vel.x -= MIN_CHANGE_VELX_PER_FRAME;
		}
		else if (vel.x >= 0 && vel.y < 0) {
			curState = State::Explode;
			vel = { 0.0f, 0.0f };
			return;
		}
		else if (vel.x < 0 && vel.y >= 0) {
			if (dy == 0 && vel.y == 0) { return; }
			else if (dy == 0 && vel.y != 0) {
				vel.x += MIN_CHANGE_VELX_PER_FRAME;
			}
			else if (dy != 0 && vel.y == 0)
			{
				vel.x += MIN_CHANGE_VELX_PER_FRAME;
			}
			else if (abs(dx) / abs(dy) < abs(vel.x) / abs(vel.y))
			{
				vel.x += MIN_CHANGE_VELX_PER_FRAME;
			}
			else
			{
				vel.x -= MIN_CHANGE_VELX_PER_FRAME;
			}
		}
		else if (vel.x >= 0 && vel.y >= 0) {
			vel.x -= MIN_CHANGE_VELX_PER_FRAME;
		}
	}
	else if (dx >= 0 && dy >= 0) {
		if (vel.x < 0 && vel.y < 0)
		{
			curState = State::Explode;
			vel = { 0.0f, 0.0f };
			return;
		}
		else if (vel.x >= 0 && vel.y < 0) {
			vel.x += MIN_CHANGE_VELX_PER_FRAME;
		}
		else if (vel.x < 0 && vel.y >= 0) {
			vel.x += MIN_CHANGE_VELX_PER_FRAME;
		}
		else if (vel.x >= 0 && vel.y >= 0) {
			if (dy == 0 && vel.y == 0) { return; }
			else if (dy == 0 && vel.y != 0) {
				vel.x += MIN_CHANGE_VELX_PER_FRAME;
			}
			else if (dy != 0 && vel.y == 0)
			{
				vel.x -= MIN_CHANGE_VELX_PER_FRAME;
			}
			else if (abs(dx) / abs(dy) < abs(vel.x) / abs(vel.y))
			{
				vel.x -= MIN_CHANGE_VELX_PER_FRAME;
			}
			else
			{
				vel.x += MIN_CHANGE_VELX_PER_FRAME;
			}
		}
	}
	UpdateVelYbyVelX(vel.x);
}

void BulletEnemyFly::UpdateVelYbyVelX(float velx)
{
	if (abs(velx) > BULLET_MOVING)
	{
		vel.x = velx/abs(velx) * BULLET_MOVING;
		vel.y = 0;
	}
	else
	{
		vel.y =  sqrt(BULLET_MOVING*BULLET_MOVING - velx * velx);		
	}
}
void BulletEnemyFly::HitCaptain()
{
	Sounds::PlayAt(SoundId::Explosion);
	this->SetState(State::Explode);
	vel = { 0.0f,0.0f };
}

