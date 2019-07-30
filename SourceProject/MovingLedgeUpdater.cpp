#include "pch.h"
#include "MovingLedgeUpdater.h"

MovingLedgeUpdater::MovingLedgeUpdater(Vector2 pos, UINT w, UINT h, Grid* grid) :
	InvisibleObject(pos, w, h)
{
	circle = std::make_shared<MovingLedge>( DEFAULT_POS_CIRCLE, Behaviors::MovingLedge_Circle );
	diagonal = std::make_shared<MovingLedge>( DEFAULT_POS_DIAGONAL, Behaviors::MovingLedge_Diagonal );
	horizontal = std::make_shared<MovingLedge>( DEFAULT_POS_HORIZONTAL, Behaviors::MovingLedge_Horizontal );
	grid->SpawnObject(circle);
	grid->SpawnObject(diagonal);
	grid->SpawnObject(horizontal);
	bboxColor = Colors::Updater;
}

void MovingLedgeUpdater::Update(float dt, const std::vector<GameObject*>& coObjects)
{
	// update 3 ledges synchronously
	circle->UpdateByUpdater(dt);
	diagonal->UpdateByUpdater(dt);
	horizontal->UpdateByUpdater(dt);
}

void MovingLedgeUpdater::OnOutOfViewPort()
{
	circle->SetPos(DEFAULT_POS_CIRCLE), circle->SetState(State::MovingLedge_Stall);
	diagonal->SetPos(DEFAULT_POS_DIAGONAL), diagonal->SetState(State::MovingLedge_Stall);
	horizontal->SetPos(DEFAULT_POS_HORIZONTAL), horizontal->SetState(State::MovingLedge_Stall);
}