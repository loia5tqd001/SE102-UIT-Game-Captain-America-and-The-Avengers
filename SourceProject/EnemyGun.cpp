#include "pch.h"
#include "EnemyGun.h"
#include "Captain.h"
#include "EnemyGunBullet.h"

EnemyGun::EnemyGun(const Vector2 & spawnPos, const Vector2 & vel, Grid *grid) : Enemy(State::EnemyGun_Stand, spawnPos, vel)
{
	animations.emplace(State::EnemyGun_Stand, Animation(SpriteId::EnemyGun_Stand, 0.1f));
	animations.emplace(State::EnemyGun_OnKnee, Animation(SpriteId::EnemyGun_OnKnee, 0.1f));
	animations.emplace(State::EnemyGun_TakeDamage, Animation(SpriteId::EnemyGun_TakeDamage, 0.1f));
	animations.emplace(State::EnemyGun_Walking, Animation(SpriteId::EnemyGun_Walking, 0.1f));
	animations.emplace(State::Explode, Animation(SpriteId::Explode, 0.1f));
	this->grid = grid;
	this->health = 1;
}

void EnemyGun::Update(float dt, const std::vector<GameObject*>& coObjects)
{
	if (curState == State::Destroyed) return;

	// regular updates
	if (curState != State::EnemyGun_TakeDamage && curState != State::Explode) pos.y += vel.y * GRAVITY; //enemy not falling down when die

	// handle collisions
	HandleCollisions(dt, coObjects);

	//counter, if take damage, flasing in 0,5s then change to explode, keep 2 sprite in 0,2 s fit and destroy
	if (curState == State::EnemyGun_TakeDamage) {
		static Counter takeDamageCounter;
		if (takeDamageCounter.CanExcuseCommand(0.5f, true))
		{
			SetState(State::Explode);
			OnFlasing();
		}
	}
	if (curState == State::Explode)
	{
		static Counter ExplodeCounter;
		if (ExplodeCounter.CanExcuseCommand(0.2f, true))
		{
			SetState(State::Destroyed);
		}
	}

	// spawn bullet every 0.4s
	SpawnBullet(4.0f);

	// update animations
	animations.at(curState).Update(dt);

	Utils::Clamp(pos.x, 0.0f, (float)Window::Instance().GetWidth(), [this]()
	{
		curState = State::Destroyed;
	});
}
void EnemyGun::SetState(State state)
{
	VisibleObject::SetState(state);

	switch (state)
	{
	case State::EnemyGun_Stand:
		vel.x = 0.0f;
		break;
	case State::EnemyGun_Walking:
		vel.x = nx * WALKING_SPEED;
		break;
	case State::EnemyGun_OnKnee:
		vel.x = 0.0f;
		OnKneeHeight(GetHeight());
		break;
	case State::EnemyGun_TakeDamage:
		vel.x = -nx * FALL_BACK;
		break;
	case State::Explode:
		vel.x = 0;
		break;
	}
}
void EnemyGun::SpawnBullet(float cycle) 
{
	static Counter curCounter;
	if (curState == State::EnemyGun_TakeDamage) return;
	if (curCounter.CanExcuseCommand(cycle, false))
	{
		grid->SpawnObject(std::make_unique<EnemyGunBullet>(Vector2{ pos.x, pos.y }, Vector2{ 0.0f, 0.0f }));
	}
}
void EnemyGun::OnKneeHeight(UINT oldHeight)
{
	assert(oldHeight > GetHeight());
	pos.y += oldHeight - GetHeight();
}
void EnemyGun::TakeDamage(UINT damage)
{
	assert(this->health > 0);
	health--;
	SetState(State::EnemyGun_TakeDamage);
}
UINT EnemyGun::GetHeight() const
{
	return animations.at(curState).GetFrameSize().GetHeight();
}
RectF EnemyGun::GetBBox() const //not sure how we will use this or not but i still put it here
{
	if (curState == State::EnemyGun_TakeDamage) return {};
	return VisibleObject::GetBBox();
}
void EnemyGun::HandleNoCollisions(float dt)
{
	pos.x += vel.x * dt;
	pos.y += vel.y * dt; //enemy will not falling down when die, just fall back
}

void EnemyGun::HandleCollisions(float dt, const std::vector<GameObject*>& coObjects)
{
	if (curState != State::EnemyGun_TakeDamage && curState != State::Explode) return;
	//this code is critical for every moving object towards
	auto coEvents = CollisionDetector::CalcPotentialCollisions(*this, coObjects, dt);
	if (coEvents.size() == 0) { HandleNoCollisions(dt); return; }

	float min_tx, min_ty, nx, ny;
	CollisionDetector::FilterCollisionEvents(coEvents, min_tx, min_ty, nx, ny);

	// NOTE: HACK: not perfect handler but we're fine
	if (coEvents.size() == 0) return; // the case object's going toward the corner

	pos.x += min_tx * vel.x * dt;
	pos.y += min_ty * vel.y * dt;

	if (nx != 0.0f) vel.x = 0.0f;
	if (ny != 0.0f) { vel.y = 0.0f; }

	//the default will cause problem, enemy can not run throught captain
	//handle (only 1 captain, maybe this code is not working i should change it later)
	for (UINT i = 0; i < coEvents.size(); i++)
	{
		const CollisionEvent& e = coEvents[i];

		if (auto captain = dynamic_cast<Captain*>(e.pCoObj))
		{
			if (nx >0 && e.nx > 0) //go right
			{
				vel.x = WALKING_SPEED;
			}
			else if (nx < 0 && e.nx < 0)
			{
				vel.x = -WALKING_SPEED;
			}
		}
	}
}

