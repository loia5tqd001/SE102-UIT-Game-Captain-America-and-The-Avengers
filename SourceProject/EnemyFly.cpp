#include "pch.h"
#include "EnemyFly.h"
#include "BulletEnemyFly.h"

EnemyFly::EnemyFly(Vector2 spawnPos, Grid* grid, Captain *cap) :
	Enemy(behavior, std::move(behaviorData), State::EnemyFly_Fly, 2, spawnPos, grid)
{
	animations.emplace(State::EnemyFly_Fly, Animation(SpriteId::EnemyFly_Fly, 0.1f));
	animations.emplace(State::EnemyFly_Falling, Animation(SpriteId::EnemyFly_Fly, 0.06f));

	//EnemyFly_BeforeExplode need many cycle, we use time counter for this, not default BeforeExplode
	//The have to advoid exploxe

	curState = State::EnemyFly_Fly;
	this->spawnPos = spawnPos;
	this->cap = cap;
}

void EnemyFly::Update(float dt, const std::vector<GameObject*>& coObjects)
{
	auto coEvents = CollisionDetector::CalcPotentialCollisions(*this, coObjects, dt);
	float _, __, ___, ____;

	if (coEvents.size()) CollisionDetector::FilterCollisionEvents(coEvents, _, __, ___, ____);
	pos.x += vel.x * dt;
	pos.y += vel.y * dt;

	for (auto& e : coEvents)
	{
		if (auto block = dynamic_cast<Block*>(e.pCoObj)) {

			switch (block->GetType())
			{
				case ClassId::RigidBlock:
					if (e.ny < 0)
					{
						SetState(State::Explode);
					}
					return;
			}
		}
	}

	SpawnBullet();

	switch (curState)
	{
		case State::EnemyFly_Fly:
		{
			//the code below stimulate its cycle, i put it rectangle to easy

			if (pos.x > cap->GetPos().x) nx = -1;
			else nx = 1;

			//static float countHorDistance = 0;
			//static float countVerDistance = 0;
			if (std::abs(countHorDistance) < HOR_DISTANCE)
			{
				if (countHorDistance >= 0) {
					countHorDistance += MOVING_SPEED*dt;
					vel.x = MOVING_SPEED;
					vel.y = 0;
					Debug::Out(1);
				}
				else {
					countHorDistance -= MOVING_SPEED*dt;
					vel.x = -MOVING_SPEED;
					vel.y = 0;
					Debug::Out(2);
				}
			}
			else
			{
				if (std::abs(countVerDistance) < VER_DISTANCE)
				{
					if (countVerDistance >= 0) {
						countVerDistance += MOVING_SPEED*dt;
						vel.y = MOVING_SPEED;
						vel.x = 0;
						Debug::Out(3);
					}
					else {
						countVerDistance -= MOVING_SPEED*dt;
						vel.y = -MOVING_SPEED;
						vel.x = 0;
						Debug::Out(4);
					}
				}
				else
				{
					if (countHorDistance > 0 && countVerDistance > 0)
					{
						countHorDistance = -1;
						countVerDistance = -1;
					}
					else
					{
						countHorDistance = 1;
						countVerDistance = 1;
					}
					Debug::Out(0);
				}
			}
			break;
		}
		case State::EnemyFly_Falling:
		{
			vel.x = 0;
			vel.y = FALLING_SPEED;
			if (fallingCounter < FALLING_TIME)
			{
				fallingCounter += GameTimer::Dt();
				if (animations.at(curState).IsDoneCycle())
					nx = -nx;
			}
			else SetState(State::Explode);
			break;
		}
		case State::Explode:
		{
			if (animations.at(curState).IsDoneCycle())
				SetState(State::Destroyed);
			vel.x = 0;
			vel.y = 0;
			break;
		}
	}
	animations.at(curState).Update(dt);
	OnFlashing();
}

void EnemyFly::SpawnBullet()
{
	if (pos.y > cap->GetPos().y) return;
	counterSpawnBullet += GameTimer::Dt();
	if (counterSpawnBullet >= 4.0f) {
		const auto bulletPos = pos + Vector2{ 24.0f, 10.0f };
		if (cap->GetPos().y > pos.y) {
			grid->SpawnObject(std::make_unique<BulletEnemyFly>(nx, this, bulletPos, cap));
			Sounds::PlayAt(SoundId::BulletNormal);
			counterSpawnBullet = 0;
		}
	}
}

void EnemyFly::TakeDamage(int damage)
{
	assert(damage > 0);
	if (isFlashing) return; // has just being damaged and is flashing, don't be too evil, give me time to recover please

	OnFlashing(true);
	health -= damage;
	if (health <= 0) {
		SetState(State::EnemyFly_Falling);
	}
	else {
		OnFlashing(true);
	}
}

void EnemyFly::SetState(State state)
{
	VisibleObject::SetState(state);
}




