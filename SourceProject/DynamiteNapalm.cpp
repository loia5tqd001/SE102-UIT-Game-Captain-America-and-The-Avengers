#include "pch.h"
#include "EnemyWizard.h"
#include "BulletFireEnemyWizard.h"
#include "BulletEnemyWizard.h"

DynamiteNapalm::DynamiteNapalm(Behaviors behavior, Data&& behaviorData, Vector2 spawnPos, Vector2 vel, int nx, Grid * grid, Captain& cap) :
	Enemy(behavior, behaviorData, State::DynamiteNapalm_FallFromTheSky, 2, spawnPos, grid),
	cap(cap)
{
	animations.emplace(State::DynamiteNapalm_FallFromTheSky, Animation(SpriteId::DynamiteNapalm_FallFromTheSky));
	animations.emplace(State::DynamiteNapalm_Standing, Animation(SpriteId::DynamiteNapalm_Standing, 2.0f));
	animations.emplace(State::DynamiteNapalm_Intact_Running, Animation(SpriteId::DynamiteNapalm_Intact_Running, 0.2f));
	animations.emplace(State::DynamiteNapalm_ThrowDynamite, Animation(SpriteId::DynamiteNapalm_ThrowDynamite, 0.5f));
	animations.emplace(State::DynamiteNapalm_Intact_Shooting, Animation(SpriteId::DynamiteNapalm_Intact_Shooting, 0.1f));
	animations.emplace(State::DynamiteNapalm_Intact_Injure, Animation(SpriteId::DynamiteNapalm_Intact_Injure, 0.1f));
	animations.emplace(State::DynamiteNapalm_Headless_Standing, Animation(SpriteId::DynamiteNapalm_Headless_Standing));
	animations.emplace(State::DynamiteNapalm_Headless_Running_Shooting, Animation(SpriteId::DynamiteNapalm_Headless_Running_Shooting));
	animations.emplace(State::DynamiteNapalm_BeforeExplode, Animation(SpriteId::DynamiteNapalm_Headless_Standing));

	this->nx = nx;
	this->vel.x = 0;
	this->vel.y = FALLING_SPEED;
	beforeExplode = State::DynamiteNapalm_BeforeExplode;
}

void DynamiteNapalm::Update(float dt, const std::vector<GameObject*>& coObjects)
{
	static Behaviors curBehavior = Behaviors::DynamiteNapalm_Fall;

	if (OnBehavior(curBehavior))
	{
		if (curBehavior == Behaviors::DynamiteNapalm_Fall)
		{
			SetState(State::DynamiteNapalm_Standing);
			curBehavior = Behaviors::DynamiteNapalm_Throw;
		}
		else if (curBehavior == Behaviors::DynamiteNapalm_Run)
		{
			SetState(State::DynamiteNapalm_Standing);
			curBehavior = Behaviors::DynamiteNapalm_Throw;
		}
		else if (curBehavior == Behaviors::DynamiteNapalm_Throw)
		{
			SetState(State::Captain_Standing);
			curBehavior = Behaviors::DynamiteNapalm_Shoot;
		}
		else if (curBehavior==Behaviors::DynamiteNapalm_Throw)
		{
			SetState(State::DynamiteNapalm_Intact_Shooting);
			curBehavior = Behaviors::DynamiteNapalm_Shoot;
		}
		else if (curBehavior==Behaviors::DynamiteNapalm_Shoot)
		{
			SetState(State::Captain_Standing);
			curBehavior = Behaviors::DynamiteNapalm_Run;
		}
	}

	UpdateAnimation(dt);
	HandleCollisions(dt, coObjects);
}

void DynamiteNapalm::SetState(State state)
{
	switch (curState)
	{
	case State::Destroyed:
		break;
	case State::DynamiteNapalm_FallFromTheSky:
		vel.x = 0.0f;
		vel.y = FALLING_SPEED;
		break;
	case State::DynamiteNapalm_Standing:
		vel.x = 0.0f;
		vel.y = 0.0f;
		break;
	case State::DynamiteNapalm_Intact_Running:
		vel.x = nx * RUNNING_SPEED;
		vel.y = 10.0f;
		break;
	case State::DynamiteNapalm_ThrowDynamite:
	case State::DynamiteNapalm_Intact_Shooting:
	case State::DynamiteNapalm_Intact_Injure:
	case State::DynamiteNapalm_Headless_Standing:
	case State::DynamiteNapalm_BeforeExplode:
		vel.x = 0.0f;
		vel.y = 0.0f;
	case State::DynamiteNapalm_Headless_Running_Shooting:
		vel.x = RUNNING_BEFORE_DEAD_SPEED;
		break;
	case State::NotExist:
	case State::Invisible:
		AssertUnreachable();
	default:
		break;
	}
	VisibleObject::SetState(state);
	Debug::out("%d", curState);
}

//return true if behavior is done
bool DynamiteNapalm::OnBehavior(Behaviors behavior)
{
	bool unhandle = false;
	switch (behavior)
	{
	case Behaviors::DynamiteNapalm_Fall:
		return onTheGround;
		break;
	case Behaviors::DynamiteNapalm_Throw:
		if (curState == State::DynamiteNapalm_Standing)
		{
			if (animations.at(curState).IsDoneCycle())
				SetState(State::DynamiteNapalm_ThrowDynamite);
			return false;
		}
		else if (curState == State::DynamiteNapalm_ThrowDynamite)
		{
			if (!animations.at(curState).IsDoneCycle())
				return false;
			else
			{
				SpawnDynamite();
				SetState(State::DynamiteNapalm_Standing);
				return true;
			}
		}
		break;
	case Behaviors::DynamiteNapalm_Shoot:
		if (curState == State::Captain_Standing)
		{
			if (!animations.at(curState).IsDoneCycle())
				return false;
			else
			{
				SetState(State::DynamiteNapalm_Intact_Shooting);
				SpawnFireBullet();
			}
		}
		else if (curState == State::DynamiteNapalm_Intact_Shooting)
		{
			if (!animations.at(curState).IsDoneCycle())
				return false;
			else
			{
				SetState(State::DynamiteNapalm_Standing);
				return true;
			}
		}
		break;
	case Behaviors::DynamiteNapalm_Run:
		if (curState == State::Captain_Standing)
		{
			if (!animations.at(curState).IsDoneCycle())
				return false;
			else
			{
				SetState(State::DynamiteNapalm_Intact_Running);
				if (nx == 1)
					posXDesination = 224.0f / 4.0f*3.0f;
				else if (nx == -1)
					posXDesination = 224.0f / 4.0f;

				//Chang nx

			}
		}
		break;
	default:
		break;
	}
	return unhandle;
	AssertUnreachable();
}

void DynamiteNapalm::HandleCollisions(float dt, const std::vector<GameObject*>& coObjects)
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
		if (auto block = dynamic_cast<Block*>(e.pCoObj))
		{
			switch (block->GetType())
			{
			case ClassId::RigidBlock:
				onTheGround = true;
				if (curState == State::DynamiteNapalm_FallFromTheSky)
					SetState(State::DynamiteNapalm_Standing);
				if (e.ny != 0) {
					pos.x += vel.x*min_tx*dt;
					pos.y += vel.y*min_ty*dt;
				}
				break;
			default:
				break;
			}
		}
	}
}

void DynamiteNapalm::SpawnDynamite()
{
}

void DynamiteNapalm::SpawnFireBullet()
{
}
