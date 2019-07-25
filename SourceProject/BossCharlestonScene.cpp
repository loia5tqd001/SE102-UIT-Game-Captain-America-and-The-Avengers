#include "pch.h"
#include "BossCharlestonScene.h"

static auto& cam = Camera::Instance();

BossCharlestonScene::BossCharlestonScene()
{
	LoadResources();

	if (!IsPause()) 
		Sounds::PlayLoop( GetBgMusic() );
}

void BossCharlestonScene::LoadResources()
{
	const auto root = GetRootJson("Resources\\Data\\scene-boss-charleston.json");

	mapDark = std::make_unique<Map>( root["dark"] );
	mapLight = std::make_unique<Map>( root["light"] );
	grid = std::make_unique<Grid>( root );
	cap = std::make_unique<Captain>( Vector2{ 32.0f, 197.0f - 45.0f } ) ;
}

void BossCharlestonScene::Update(float dt)
{
	grid->UpdateCells();
	for (auto& obj : grid->GetObjectsInViewPort())
		obj->Update(dt);
	cap->Update(dt, grid->GetObjectsInViewPort());
	cam.ClampWithin( mapDark->GetWorldBoundary() );
}

void BossCharlestonScene::Draw()
{
	if (isDark) mapDark->Render();
	else mapLight->Render();
	for (auto& obj: grid->GetObjectsInViewPort())
		obj->Render();
	cap->Render();
	grid->RenderCells();
}

void BossCharlestonScene::OnKeyUp(BYTE keyCode)
{
	cap->OnKeyUp(keyCode);
}

void BossCharlestonScene::OnKeyDown(BYTE keyCode)
{
	switch (keyCode)
	{
		case VK_SPACE:
			isDark = !isDark;
			break;

		case VK_RETURN:
			DoTransitionScene(Scene::Pittsburgh);
			break;
	}
	cap->OnKeyDown(keyCode);
}
