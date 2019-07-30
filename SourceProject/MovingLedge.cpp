#include "pch.h"
#include "MovingLedge.h"
#define PI 3.14159265

MovingLedge::MovingLedge(Vector2 pos, Behaviors behavior) :
	VisibleObject(State::MovingLedge_Stop, pos),
	behavior(behavior)
{
	animations.emplace(State::MovingLedge_Stop, Animation(SpriteId::MovingLedge, 0.15f));
	animations.emplace(State::MovingLedge_Moving, Animation(SpriteId::MovingLedge, 0.05f));
}

void MovingLedge::OnCircleMoving(float dt)
{
	//eclipse(580,612) 30 52
	static int counter = 0;
	static float x = 52*cos(counter / 10 * dt);
	static float y = 30*cos(counter / 10 * dt - PI / 4);
	pos.x = 580 + x;
	pos.y = 612 + y;
}

void MovingLedge::OnDiagonalMoving(float dt)
{
}

void MovingLedge::OnHorizontalMoving(float dt)
{
}

void MovingLedge::UpdateByUpdater(float dt)
{
	animations.at(curState).Update(dt);

	if (behavior == Behaviors::MovingLedge_Circle)
	{
		OnCircleMoving(dt);
	}
	else if (behavior == Behaviors::MovingLedge_Diagonal)
	{
		OnDiagonalMoving(dt);
	}
	else if (behavior == Behaviors::MovingLedge_Horizontal)
	{
		OnHorizontalMoving(dt);
	}
}