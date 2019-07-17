#include "pch.h"
#include "Shield.h"


Shield::Shield(Captain *captain) : VisibleObject(State::Invisible,captain->GetPos())
{
	animations.emplace(State::Shield_Up, Animation(SpriteId::Shield_Up, 0.1f));
	animations.emplace(State::Shield_Down, Animation(SpriteId::Shield_Down, 0.1f));
	animations.emplace(State::Shield_Straight, Animation(SpriteId::Shield_Straight, 0.1f));
	animations.emplace(State::Shield_Side, Animation(SpriteId::Shield_Side, 0.1f));


	//TODO
	//Set Velocity
	vel.y = 0;			//caculate y base on x, don't use this thing
	vel.x = 0;
	UpdateByCapState(State::Captain_Standing, captain->GetPos());
	curState = State::Shield_Straight;
	
	//Caculate shield position depend on Cap's Position
	//pos.x
}

void Shield::SetState(State state)
{
	const auto oldHeight = GetHeight();
	VisibleObject::SetState(state);
	pos.y += oldHeight - GetHeight();

	//NOTE:the code below may needed for later
	//switch (state)
	//{
	//case State::Shield_Straight: //shield is on cap for sure
	//	vel = { 0.0f, 0.0f };
	//	break;
	//case State::Shield_Side: //shield is on cap for sure, and shield can take bullet
	//	vel = { 0.0f, 0.0f };
	//	break;
	//case State::Shield_Down: //shield is on cap for sure, and shield deal 3 damage to enemy
	//	vel = { 0.0f, 0.0f };
	//	break;
	//case State::Shield_Up: //shield is flying or on cap, if it flying deal 1 damage to enemy, if it on cap it can take bullet
	//	vel = { 0.0f, 0.0f };
	//	break;
	//}
}
void Shield::Update(float dt, const std::vector<GameObject*>& coObjects)
{
	nx = cap->GetNx();

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
			static float flagDistance;
			flagDistance = distance;
			if (flagDistance < MAX_DISTANCE)
			{
				flagDistance += SPEED;
				pos.x += nx * SPEED *dt;
			}
			else
			{
				flagDistance -= SPEED;
				pos.x -= nx * SPEED *dt;
				CalculateVely();
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
	UpdateByCapState(State::Captain_Throw, cap->GetPos());
	isOnCaptain = false; //go to the moving code in update
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
	distance = 0;
	isOnCaptain = true;
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

void Shield::CalculateVely()
{
	//HACK:this moving may not perfect, we could calculate a math function, but i 'd like to if else a lot till it looks right
	float destinationY = cap->GetPos().y - cap->GetHeight() / 2;
	int distanceY = destinationY - pos.y;
	// remember s=t*v
	float timeX = distance / SPEED;
	float speedY = distanceY / timeX;
	vel.y += speedY;
}


