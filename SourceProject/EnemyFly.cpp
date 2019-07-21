#include "pch.h"
#include "EnemyFly.h"

EnemyFly::EnemyFly(Vector2 spawnPos, Grid* grid, Captain *cap) :
	Enemy(behavior, std::move(behaviorData), State::EnemyFly_Fly, 1, spawnPos, grid)
{
	animations.emplace(State::EnemyFly_Fly, Animation(SpriteId::EnemyFly_Fly, 0.2f));
	animations.emplace(State::EnemyFly_Falling, Animation(SpriteId::EnemyFly_Fly, 0.06f));

	//EnemyFly_BeforeExplode need many cycle, we use time counter for this, not default BeforeExplode
	//The have to advoid exploxe

	curState = State::EnemyFly_Fly;
	this->spawnPos = spawnPos;
	this->cap = cap;
}

void EnemyFly::Update(float dt, const std::vector<GameObject*>& coObjects)
{
	pos.x += vel.x*dt;
	pos.y += vel.y*dt;

	switch (curState)
	{
		case State::EnemyFly_Fly:
		{
			//the code below stimulate its cycle, i put it rectangle to easy

			if (pos.x > cap->GetPos().x) nx = -1;
			else nx = 1;

			static float countHorDistance = 0;
			static float countVerDistance = 0;
			if (std::abs(countHorDistance) < HOR_DISTANCE)
			{
				if (countHorDistance >= 0) {
					countHorDistance += MOVING_SPEED;
					vel.x = MOVING_SPEED;
					vel.y = 0;
				}
				else {
					countHorDistance -= MOVING_SPEED;
					vel.x = -MOVING_SPEED;
					vel.y = 0;
				}
			}
			else
			{
				if (std::abs(countVerDistance) < VER_DISTANCE)
				{
					if (countVerDistance >= 0) {
						countVerDistance += MOVING_SPEED;
						vel.y = MOVING_SPEED;
						vel.x = 0;
					}
					else {
						countVerDistance -= MOVING_SPEED;
						vel.y = -MOVING_SPEED;
						vel.x = 0;
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
}

void EnemyFly::SpawnBullet()
{
	//TODO
}

void EnemyFly::TakeDamage(int damage)
{
	assert(damage > 0);
	if (isFlashing) return; // has just being damaged and is flashing, don't be too evil, give me time to recover please

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



