#include "pch.h"
#include "ElectricTrap.h"

float ElectricTrap::deltaTime = 0.0f;
float ElectricTrap::holdingTime = 0.0f;
State ElectricTrap::syncState = State::NotExist;

ElectricTrap::ElectricTrap(State initState, Vector2 pos) :VisibleObject(State::ElectricTrap_Active, pos)
{
	animations.emplace(State::ElectricTrap_Active, Animation(SpriteId::ElectricTrap_Active, 0.1f));
	animations.emplace(State::Invisible, Animation(SpriteId::Invisible, sleepTime));
}

void ElectricTrap::Update(float dt, const std::vector<GameObject*>& coObjects)
{
	if (countUpdateTime == 0)
	{
		countUpdateTime++;
		deltaTime = dt;
		holdingTime += dt;


		if (syncState!=State::NotExist)
		{
			curState = syncState;
		}

		if (curState==State::ElectricTrap_Active&&holdingTime>holdTime)
		{
			curState = State::Invisible;
			syncState = State::Invisible;
			holdingTime = 0;
		}
		else if (curState==State::Invisible&&holdingTime>sleepTime)
		{
			curState = State::ElectricTrap_Active;
			syncState = State::ElectricTrap_Active;
			holdingTime = 0;
		}

		animations.at(curState).Update(deltaTime);
	}
	else if (countUpdateTime == 1)
	{
		if (syncState!=State::NotExist)
		{
			curState = syncState;
		}

		animations.at(curState).Update(deltaTime);
		countUpdateTime = 0;
	}
}
