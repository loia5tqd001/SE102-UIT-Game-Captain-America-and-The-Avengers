#include "pch.h"
#include "Shield.h"


Shield::Shield(Captain *captain) : VisibleObject(State::Shield_Straight,captain->GetPos())
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
			HandleUpCollison(dt, coObjects);
			break;
		}
		else {
			//to the max_distance
			static float flagDistance = 0;
			static bool turnBack;
			if (distance < MAX_DISTANCE)
			{
				turnBack = true;
				flagDistance += SPEED*dt;
				distance += SPEED*dt;
				pos.x += nx*SPEED*dt; // nx is not allow to use in this 
			}
			else
			{
				if (turnBack)
				{ //i tried flag with (flagDistance>MAX_DISTANCE) then flagDistance-= SPEED but it cause bug: it may do the functiton 2 times
					turnBack = false;
					nx = -nx; //only do this once}
				}
				flagDistance -= SPEED*dt; //we need distance for if else checking, and flagDistance to update pos.y
				pos.x += nx*SPEED*dt;
				CalculateVely(dt); //update pos.y
				HandleCaptainCollison(dt, coObjects); //distance = 0
			}
			break;
		}
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
		else if (capState == State::Captain_OnOneKnee)
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
		else if (capState == State::Captain_Jump)
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
			//we dont have sprite to calculate this yet
			SetState(State::Shield_Up);
		}
		else if (capState == State::Captain_Climb)
		{
			SetState(State::Invisible);
		}
		else if (capState == State::Captain_Spin)
		{
			SetState(State::Invisible);
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
}

void Shield::HandleUpCollison(float dt, const std::vector<GameObject*>& coObjects)
{
}

void Shield::HandleBottomCollison(float dt, const std::vector<GameObject*>& coObjects)
{
}

void Shield::CalculateVely(float dt)
{
	//HACK:this moving may not perfect, we could calculate a math function, but i 'd like to if else a lot till it looks right
	float destinationY = cap->GetPos().y + 10;
	int distanceY = destinationY - pos.y;
	// remember s=t*v
	float timeX = distance / SPEED;
	float speedY = distanceY / timeX;
	pos.y += speedY * dt;
}


