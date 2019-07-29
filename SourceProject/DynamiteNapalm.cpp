#include "pch.h"
#include "EnemyWizard.h"
#include "BulletFireEnemyWizard.h"
#include "BulletEnemyWizard.h"

DynamiteNapalm::DynamiteNapalm(Behaviors behavior, Data&& behaviorData, Vector2 spawnPos, Vector2 vel, int nx, Grid * grid, Captain& cap) :
	Enemy(behavior, behaviorData, State::DynamiteNapalm_FallFromTheSky, 2, spawnPos, grid),
	cap(cap)
{
	animations.emplace(State::DynamiteNapalm_FallFromTheSky, Animation(SpriteId::DynamiteNapalm_FallFromTheSky));
	animations.emplace(State::DynamiteNapalm_Standing, Animation(SpriteId::DynamiteNapalm_Standing, 0.5f));
	animations.emplace(State::DynamiteNapalm_Intact_Running, Animation(SpriteId::DynamiteNapalm_Intact_Running, 0.2f));
	animations.emplace(State::DynamiteNapalm_ThrowDynamite, Animation(SpriteId::DynamiteNapalm_ThrowDynamite, 0.5f));
	animations.emplace(State::DynamiteNapalm_Intact_Shooting, Animation(SpriteId::DynamiteNapalm_Intact_Shooting, 0.2f));
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

	if (OnBehavior(curBehavior, dt))
	{
		if (curBehavior == Behaviors::DynamiteNapalm_Fall)
		{
			SetState(State::DynamiteNapalm_Standing);
			curBehavior = Behaviors::DynamiteNapalm_Throw;
		}
		else if (curBehavior == Behaviors::DynamiteNapalm_Throw)
		{
			SetState(State::DynamiteNapalm_Standing);
			curBehavior = Behaviors::DynamiteNapalm_Shoot;
			numFireBulletFired = 0;
		}
		else if (curBehavior == Behaviors::DynamiteNapalm_Shoot)
		{
			if (numFireBulletFired < 2)
			{
				curBehavior = Behaviors::DynamiteNapalm_Shoot;
			}
			else
			{
				SetState(State::DynamiteNapalm_Standing);
				curBehavior = Behaviors::DynamiteNapalm_Run;
			}
		}
		else if (curBehavior == Behaviors::DynamiteNapalm_Run)
		{
			SetState(State::DynamiteNapalm_Standing);
			curBehavior = Behaviors::DynamiteNapalm_Throw;
		}

		//else if (curBehavior == Behaviors::DynamiteNapalm_Throw)
		//{
		//	SetState(State::DynamiteNapalm_Intact_Shooting);
		//	curBehavior = Behaviors::DynamiteNapalm_Shoot;
		//}
	}

	UpdateAnimation(dt);
	HandleCollisions(dt, coObjects);
}

void DynamiteNapalm::SetState(State state)
{
	switch (state)
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
		break;
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

	switch (curState)
	{
	case State::Invisible:
		Debug::out("Invisible\n");

		break;
	case State::Destroyed:
		Debug::out("Destroyed\n");

		break;
	case State::DynamiteNapalm_Standing:
		Debug::out("DynamiteNapalm_Standing\n");
		break;
	case State::DynamiteNapalm_Intact_Running:
		Debug::out("DynamiteNapalm_Intact_Running\n");

		break;
	case State::DynamiteNapalm_ThrowDynamite:
		Debug::out("DynamiteNapalm_ThrowDynamite\n");

		break;
	case State::DynamiteNapalm_Intact_Shooting:
		Debug::out("DynamiteNapalm_Intact_Shooting\n");

		break;
	case State::DynamiteNapalm_Intact_Injure:
		Debug::out("DynamiteNapalm_Intact_Injure\n");

		break;
	case State::DynamiteNapalm_Headless_Standing:
		Debug::out("DynamiteNapalm_Headless_Standing\n");

		break;
	case State::DynamiteNapalm_Headless_Running_Shooting:
		Debug::out("DynamiteNapalm_Headless_Running_Shooting\n");

		break;
	case State::DynamiteNapalm_BeforeExplode:
		Debug::out("DynamiteNapalm_BeforeExplode\n");

		break;
	case State::NotExist:
		Debug::out("NotExist\n");

		break;
	default:
		break;
	}
}

//return true if behavior is done
bool DynamiteNapalm::OnBehavior(Behaviors behavior, float dt)
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
			if (cap.GetPos().x < pos.x)
			{
				this->nx = -1;
				//vel.x = this->nx*RUNNING_SPEED;
			}
			else
			{
				this->nx = 1;
				//vel.x = this->nx*RUNNING_SPEED;
			}

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
		if (curState == State::DynamiteNapalm_Standing)
		{
			if (!animations.at(curState).IsDoneCycle())
				return false;
			else
			{
				SetState(State::DynamiteNapalm_Intact_Shooting);
				SpawnFireBullet();
				numFireBulletFired++;
				return false;
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
		if (curState == State::DynamiteNapalm_Standing)
		{
			if (!animations.at(curState).IsDoneCycle())
			{
				return false;
			}
			else
			{
				SetState(State::DynamiteNapalm_Intact_Running);
				if (cap.GetPos().x < this->pos.x)
				{
					this->nx = -1;
				}
				else this->nx = 1;
				vel.x = RUNNING_SPEED * this->nx;
				return false;
			}
		}
		else if (curState == State::DynamiteNapalm_Intact_Running)
		{
			holdingDistance += std::abs(vel.x*dt);
			if (holdingDistance <= maxDistance_IntactRunning)
			{
				return false;
			}
			else
			{
				holdingDistance = 0.0f;
				return true;
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
				pos.y += min_ty * vel.y* dt;
				onTheGround = true;
				if (e.nx != 0)
				{
					this->nx = -this->nx;
					vel.x *= -1;
				}
				pos.x += min_tx * vel.x* dt;

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
