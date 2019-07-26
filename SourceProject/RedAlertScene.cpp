#include "pch.h"
#include "RedAlertScene.h"

static auto& cam = Camera::Instance();

RedAlertScene::RedAlertScene()
{
	LoadResources();

	if (!IsPause()) 
		Sounds::PlayLoop( GetBgMusic() );
}

void RedAlertScene::LoadResources()
{
	const auto root = GetRootJson("Resources\\Data\\scene-red-alert.json");

	map = std::make_unique<Map>( root );
	grid = std::make_unique<Grid>( root );
	cap = std::make_unique<Captain>( Vector2{ 32.0f, 196.0f - 45.0f },grid.get() ) ;
}



void RedAlertScene::Update(float dt)
{
	map->UpdateAnimatedTiles(dt);
	grid->UpdateCells();
	for (auto& obj : grid->GetObjectsInViewPort())
		obj->Update(dt);
	cap->Update(dt, grid->GetObjectsInViewPort());
	cam.ClampWithin( map->GetWorldBoundary() );
}

void RedAlertScene::Draw()
{
	map->Render();
	for (auto& obj: grid->GetObjectsInViewPort())
		obj->Render();
	cap->Render();
	grid->RenderCells();
}

void RedAlertScene::OnKeyDown(BYTE keyCode)
{
	switch (keyCode)
	{
		case VK_RETURN:
			DoTransitionScene(Scene::Charleston);
			break;
	}
	cap->OnKeyDown(keyCode);
}

void RedAlertScene::OnKeyUp(BYTE keyCode)
{
	cap->OnKeyUp(keyCode);
}

