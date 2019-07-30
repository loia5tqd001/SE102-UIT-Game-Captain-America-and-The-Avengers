#include "pch.h"
#include "MovingLedge.h"

MovingLedge::MovingLedge(Vector2 pos, Behaviors behavior) :
	VisibleObject(State::MovingLedge_Stop, pos),
	behavior(behavior)
{
	animations.emplace(State::MovingLedge_Stop, Animation(SpriteId::MovingLedge, 0.15f));
	animations.emplace(State::MovingLedge_Moving, Animation(SpriteId::MovingLedge, 0.05f));
}

void MovingLedge::OnCircleMoving(float dt)
{

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