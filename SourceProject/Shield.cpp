#include "pch.h"
#include "Shield.h"
#include "Enemy.h"
#include "BulletEnemyGun.h"

Shield::Shield(Captain *captain) : VisibleObject(State::Shield_Straight, captain->GetPos())
{
	animations.emplace(State::Shield_Up, Animation(SpriteId::Shield_Up, 0.1f));
	animations.emplace(State::Shield_Down, Animation(SpriteId::Shield_Down, 0.1f));
	animations.emplace(State::Shield_Straight, Animation(SpriteId::Shield_Straight, 0.1f));
	animations.emplace(State::Shield_Side, Animation(SpriteId::Shield_Side, 0.1f));
	UpdateByCapState(State::Captain_Standing, captain->GetPos());
	this->cap = captain;
}

void Shield::SetState(State state)
{
	VisibleObject::SetState(state);
}

void Shield::Update(float dt, const std::vector<GameObject*>& coObjects)
{
	if (isOnCaptain)
		nx = cap->GetNx();// get nx to flip posx

	switch (curState)
	{
	case State::Invisible:
		break;
	case State::Destroyed:
		break;
	case State::Shield_Side:
	{
		UpdateByCapState(cap->GetState(), cap->GetPos());
		HandleSideCollison(dt, coObjects);
		break;
	}
	case State::Shield_Down:
	{
		UpdateByCapState(cap->GetState(), cap->GetPos());
		HandleBottomCollison(dt, coObjects);
		break;
	}
	case State::Shield_Straight:
	{
		UpdateByCapState(cap->GetState(), cap->GetPos());
		break;
	}
	case State::Shield_Up:
	{
		if (isOnCaptain)
		{
			UpdateByCapState(cap->GetState(), cap->GetPos());
		}
		else {
			//to the max_distance
			static float flagDistance = 0;
			static bool turnBack;
			if (distance < MAX_DISTANCE)
			{
				turnBack = true;
				flagDistance += SPEED * dt;
				distance += SPEED * dt;
				pos.x += nx * SPEED*dt;
			}
			else
			{
				if (turnBack)
				{ //i tried flag with (flagDistance>MAX_DISTANCE) then flagDistance-= SPEED but it cause bug: it may do the functiton 2 times
					turnBack = false;
					nx = -nx; //only do this once}
				}
				flagDistance -= SPEED * dt; //we need distance for if else checking, and flagDistance to update pos.y

				//Caculate pos.y base on pos.x by using vector and distance ratio
				Vector2 shieldCenter = this->GetBBox().GetCenter();
				Vector2 capCenter = cap->GetBBox().GetCenter();
				Vector2 trans = capCenter - shieldCenter;
				float ratio = (SPEED * dt) / std::abs(capCenter.x-shieldCenter.x);
				trans.x *= ratio;
				trans.y *= ratio;

				pos.x += nx * SPEED*dt;
				pos.y += trans.y;

				HandleCaptainCollison(dt, coObjects); //distance = 0
			}
		}
		HandleUpCollison(dt, coObjects);
		break;
	}
	default:
		break;
	}
}

void Shield::ThrowAway()
{
	if (isOnCaptain) { //cannot throw when shield is not on cap
		UpdateByCapState(State::Captain_Throw, cap->GetPos());
		isOnCaptain = false; //go to the moving code in update
		nx = cap->GetNx();
		cap->setShieldOn(false);
	}
}

void Shield::UpdateByCapState(State capState, Vector2 capPos)
{
	if (isOnCaptain) {

		if (capState == State::Captain_Standing)
		{
			pos.x = capPos.x + 18;
			pos.y = capPos.y + 8;
			SetState(State::Shield_Straight);
		}
		else if (capState == State::Captain_Moving)
		{
			pos.x = capPos.x + 18;
			pos.y = capPos.y + 7;
			SetState(State::Shield_Straight);
		}
		else if (capState == State::Captain_Sitting)
		{
			pos.x = capPos.x + 19;
			pos.y = capPos.y + 10;
			SetState(State::Shield_Side);
		}
		else if (capState == State::Captain_OnTwoKnee)
		{
			pos.x = capPos.x + 24;
			pos.y = capPos.y - 3;
			SetState(State::Shield_Down);
		}
		else if (capState == State::Captain_Jump||capState==State::Captain_Falling)
		{
			pos.x = capPos.x - 1;
			pos.y = capPos.y + 3;
			SetState(State::Shield_Side);
		}
		else if (capState == State::Captain_JumpKick)
		{
			pos.x = capPos.x - 3;
			pos.y = capPos.y + 5;
			SetState(State::Shield_Side);
			nx = -nx;
			flipPosx(); //the only state that shield is flip already, so flip 2 times then it will back in the work
		}
		else if (capState == State::Captain_SitPunch)
		{
			pos.x = capPos.x + 7;
			pos.y = capPos.y + 7;
			SetState(State::Shield_Side);
		}
		else if (capState == State::Captain_Throw)
		{
			pos.x = capPos.x - 5;
			pos.y = capPos.y + 1;
			SetState(State::Shield_Up);
		}
		else if (capState == State::Captain_Smash)
		{
			SetState(State::Invisible);
		}
		else if (capState == State::Captain_Climb)
		{
			//we dont have sprite to calculate this yet
		}
		else if (capState == State::Captain_Spin)
		{
			pos.x = capPos.x - 2;
			pos.y = capPos.y + 16;
			SetState(State::Invisible);
		} else if (capState==State::Captain_Sitting)
		{
			pos.x = capPos.x + 18;
			pos.y = capPos.y + 8;
			SetState(State::Shield_Straight);
		} else if (capState==State::Captain_LookUp)
		{
			//Todo: do it
			pos.x = capPos.x + 3;
			pos.y = capPos.y - 4;
			SetState(State::Shield_Up);
		}
		else if (capState == State::Captain_LookUp)
		{
			pos.x = capPos.x + 7;
			pos.y = capPos.y - 1;
			SetState(State::Shield_Up);
		}
		if (nx < 0)
		{
			flipPosx(); //case that captain turn left
		}
	}
}
void Shield::flipPosx()
{
	//only call when captain turn left
	//dont worry i 've done the math
	pos.x = 2 * cap->GetPos().x - pos.x + cap->GetWidth() - this->GetWidth();
}

void Shield::HandleCaptainCollison(float dt, const std::vector<GameObject*>& coObjects)
{
	if (nx < 0 && pos.x < cap->GetPos().x)
	{
		UpdateByCapState(cap->GetState(), cap->GetPos());
		isOnCaptain = true;
		distance = 0;
		cap->setShieldOn(true);
	}
	else if (nx > 0 && pos.x > cap->GetPos().x)
	{
		UpdateByCapState(cap->GetState(), cap->GetPos());
		isOnCaptain = true;
		distance = 0;
		cap->setShieldOn(true);
	}
}

void Shield::HandleSideCollison(float dt, const std::vector<GameObject*>& coObjects)
{
	auto coEvents = CollisionDetector::CalcPotentialCollisions(*this, coObjects, dt);
	if (coEvents.size() == 0) return;
	if (isOnCaptain) //deflect bullet
	{
		for (UINT i = 0; i < coEvents.size(); i++)
		{
			const CollisionEvent& e = coEvents[i];

			if (auto bullet = dynamic_cast<BulletEnemyGun*>(e.pCoObj))
			{
				if (e.nx > 0.0f && this->nx > 0 || e.nx < 0.0f && this->nx < 0)
				{
					bullet->Reflect();
				}
			}
		}
	}
}

void Shield::HandleUpCollison(float dt, const std::vector<GameObject*>& coObjects)
{
	auto coEvents = CollisionDetector::CalcPotentialCollisions(*this, coObjects, dt);
	if (coEvents.size() == 0) return;
	if (isOnCaptain) //deflect bullet, this is use for bulletenemyboss
	{   //TODO: change this to bulletenemyboss
		for (UINT i = 0; i < coEvents.size(); i++)
		{
			const CollisionEvent& e = coEvents[i];

			if (auto bullet = dynamic_cast<BulletEnemyGun*>(e.pCoObj)) //
			{
				if (e.ny < 0.0f)
				{
					bullet->Reflect();
				}
			}
		}
	}
	else //cause damage to enemy
	{
		for (UINT i = 0; i < coEvents.size(); i++)
		{
			const CollisionEvent& e = coEvents[i];

			if (auto enemy = dynamic_cast<Enemy*>(e.pCoObj))
			{
				enemy->TakeDamage(1);
			}
		}
	}
}

void Shield::HandleBottomCollison(float dt, const std::vector<GameObject*>& coObjects)
{
	auto coEvents = CollisionDetector::CalcPotentialCollisions(*this, coObjects, dt);
	if (coEvents.size() == 0) return;
	if (isOnCaptain)
	{
		for (UINT i = 0; i < coEvents.size(); i++)
		{
			const CollisionEvent& e = coEvents[i];

			if (auto enemy = dynamic_cast<Enemy*>(e.pCoObj)) //
			{
				if (e.ny > 0.0f)
				{
					enemy->TakeDamage(3);
				}
			}
		}
	}
}
