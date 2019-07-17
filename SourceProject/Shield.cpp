#include "pch.h"
#include "Shield.h"

Shield::Shield(Vector2 capPos):VisibleObject(State::Invisible,capPos)
{
	//TODO
	//Set Velocity
	vel.y = 0;			//caculate y base on x, don't use this thing
	vel.x = 0.5f;
	maximumDistance = 120.0f;
	curState = State::Shield_Holding;
	stateAtLastFrame = curState;
	
	//Caculate shield position depend on Cap's Position
}

void Shield::Update(float dt, const Captain& cap, const std::vector<GameObject*>& coObjects)
{
	switch (curState)
	{
	case State::Invisible:
		break;
	case State::Destroyed:
		break;
	case State::Shield_Holding:
	{
		//Get Cap's Position

		//Get Cap's state

		//Set position
		
		break;
	}
	case State::Shield_BeingThrown:
	{
		if (stateAtLastFrame == State::Shield_BeingThrown)
		{
			this->SetState(State::Shield_FlyingAway);
		}
		break;
	}

	case State::Shield_FlyingAway:
	{
		//Toi diem xa nhat
		pos.x = pos.x + vel.x*dt;
		if (pos.x>=maximumDistance)
		{
			vel.x = -vel.x;
			SetState(State::Shield_FlyingBack);
		}
		stateAtLastFrame = State::Shield_FlyingAway;
		break;
	}
	case State::Shield_FlyingBack:
	{
		//Get rectangle
		RectF capRect = cap.GetBBox();
		RectF shieldRect = this->GetBBox();

		Vector2 capCenterPoint((capRect.left + capRect.right) / 2, (capRect.top + capRect.bottom) / 2);
		Vector2 shieldCenterPoint((shieldRect.left + shieldRect.right) / 2, (shieldRect.top + shieldRect.bottom) / 2);
		
		//Prevent devide by zero
		if (shieldCenterPoint.x==capCenterPoint.x)
		{
			SetState(State::Shield_Holding);
		}

		pos.x = pos.x + vel.x*dt;
		pos.y = (pos.x - capCenterPoint.x) / (shieldCenterPoint.x - capCenterPoint.x)*(shieldCenterPoint.y - capCenterPoint.y) + capCenterPoint.y; //Line equation
		stateAtLastFrame = State::Shield_FlyingBack;

		//Check if shield arrived
		if (vel.x<0)
		{
			if (pos.x<cap.GetBBox().left)
			{
				//Todo
				//Init default state
				SetState(State::Shield_Holding);
			}
		}
		else
		{
			if (pos.x > cap.GetBBox().left)
			{
				//Todo
				//Init default state
				SetState(State::Shield_Holding);
			}
		}
		break;
	}
	default:
		break;
	}
	stateAtLastFrame = curState;
}

Shield::~Shield()
{
}
