#include "pch.h"
#include "BulletDynamite.h"


BulletDynamite::BulletDynamite(int nx, Enemy *enemy, const Vector2& spawnPos, Vector2 vel,bool farTarget) :
	Bullet(State::DynamiteNapalm_Dynamite, 3, spawnPos, { nx * BULLET_MOVING, 0.0f }, nx, enemy)
{
	animations.emplace(State::DynamiteNapalm_Dynamite, Animation(SpriteId::DynamiteNapalm_Dynamite, 0.1f));
	animations.emplace(State::Explode, Animation(SpriteId::Explode, 0.2f));
	if (nx < 0) GameObject::FlipPosXToLeft(pos.x, enemy->GetPosX(), this->GetWidth(), enemy->GetWidth());
	if (farTarget)
	{
		Target(true);
	}
	else
		Target(false);
}

void BulletDynamite::Update(float dt, const std::vector<GameObject*>& coObjects)
{
	if (holdtime<TIME_TO_THROW)
	{
		holdtime += dt;
		if (triggered)
		{
			SetState(State::Explode);
			holdtime = TIME_TO_THROW;
		}
		return;
	}

	if (triggered)
	{
		SetState(State::Explode);
	}

	if (curState == State::Explode)
	{
		if (animations.at(curState).IsDoneCycle())
		{
			SetState(State::Destroyed);
			if (triggered)
			{
				enemy->TakeDamage(4);
			}
			return;
		}
		else
		{
			goto label1;
		}
	}

	holdtime += dt;
	vel.x = nx * Speed * cos(pi / 180 * Alpha);
	vel.y = (holdtime - TIME_TO_THROW) * Gravity - Speed * sin(pi / 180 * Alpha);

	HandleCollisions(dt, coObjects);
	label1: animations.at(curState).Update(dt);
}

RectF BulletDynamite::GetBBox() const
{
	return VisibleObject::GetBBox();
}

void BulletDynamite::HandleCollisions(float dt, const std::vector<GameObject*>& coObjects)
{
	auto coEvents = CollisionDetector::CalcPotentialCollisions(*this, coObjects, dt);
	if (coEvents.size() == 0)
	{
		pos.x += vel.x*dt;
		pos.y += vel.y*dt;
		return;
	}

	float min_tx, min_ty, nx, ny;
	CollisionDetector::FilterCollisionEvents(coEvents, min_tx, min_ty, nx, ny);
	if (coEvents.size() == 0)
		return;

	for (auto&e : coEvents)
	{
		if (auto cap = dynamic_cast<Captain*>(e.pCoObj))
		{
			SetState(State::Explode);
		}
		else if (auto block = dynamic_cast<Block*>(e.pCoObj))
		{
			switch (block->GetType())
			{
			case ClassId::RigidBlock:
			{
				if(e.ny!=0)
					SetState(State::Explode);
				if (e.nx!=0)
				{
					this->nx = -this->nx;
				}
			}
			default:
				break;
			}
		}
	}
}

inline void BulletDynamite::Target(bool isFar)
{
	if (isFar)
	{
		Alpha = 75.0f;
		Speed = 320.0f;
	}
	else
	{
		Alpha = 75.0f;
		Speed = 150.0f;
	}
}
