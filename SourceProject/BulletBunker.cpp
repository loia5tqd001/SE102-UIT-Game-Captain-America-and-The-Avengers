#include "pch.h"
#include "BulletBunker.h"

BulletBunker::BulletBunker(Vector2 spawnPos, Vector2 vel)
	: Bullet(State::Bullet_Bunker, 1, spawnPos, vel, 1, nullptr)
{
	animations.emplace(State::Bullet_Bunker, Animation(SpriteId::Bullet_Bunker));

	if (GetBBox().IsIntersect(Camera::Instance().GetBBox()))
	{
		Sounds::PlayAt(SoundId::BulletNormal);
	}
}

void BulletBunker::Update(float dt, const std::vector<GameObject*>& coObjects)
{
	auto coEvents = CollisionDetector::CalcPotentialCollisions(*this, coObjects, dt);
	float min_tx, min_ty, nx, ny;

	if (coEvents.size())
	CollisionDetector::FilterCollisionEvents(coEvents, min_tx, min_ty, nx, ny);
	pos.x += vel.x * dt;
	pos.y += vel.y * dt;

	for (auto& e : coEvents)
	{
		if (auto block = dynamic_cast<Block*>(e.pCoObj)) {

			switch (block->GetType())
			{
				case ClassId::NextMap:
				case ClassId::Switch:
				case ClassId::Door:
				case ClassId::ClimbableBar:

				case ClassId::PassableLedge:
					if (e.ny < 0)
						SetState(State::Destroyed);
					break;
				case ClassId::DamageBlock:
				case ClassId::RigidBlock:
					SetState(State::Destroyed);
					break;
			}
		}
	}
}

void BulletBunker::HitCaptain()
{
	SetState(State::Destroyed);
}
