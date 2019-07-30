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
	bboxColor = Colors::MyPoisonGreen;
}

void MovingLedgeUpdater::Update(float dt, const std::vector<GameObject*>& coObjects)
{
	// add ledges to grid if they're removed from grid and currently in viewport, unique() means only updater holds the ledge, the grid doesn't
	//if ( circle    .unique() && circle    ->IsInViewPort() ) grid->SpawnObject(circle);
	//if ( diagonal  .unique() && diagonal  ->IsInViewPort() ) grid->SpawnObject(diagonal);
	//if ( horizontal.unique() && horizontal->IsInViewPort() ) grid->SpawnObject(horizontal);

	// update 3 ledges synchronously
	circle->UpdateByUpdater(dt);
	diagonal->UpdateByUpdater(dt);
	horizontal->UpdateByUpdater(dt);
}

void MovingLedgeUpdater::OnOutOfViewPort()
{
	circle->SetPos(DEFAULT_POS_CIRCLE);
	diagonal->SetPos(DEFAULT_POS_DIAGONAL);
	horizontal->SetPos(DEFAULT_POS_HORIZONTAL);
}