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
	curMap = mapLight.get();
}

void BossCharlestonScene::Update(float dt)
{
	cam.ClampWithin( curMap->GetWorldBoundary() );
}

void BossCharlestonScene::Draw()
{
	curMap->Render();

	const auto& wnd = Window::Instance();
	if (wnd.IsKeyPressed(VK_LEFT))
		cam.MoveBy( { -5.0f, 0.0f });
	if (wnd.IsKeyPressed(VK_UP))
		cam.MoveBy( { 0.0f, -5.0f });
	if (wnd.IsKeyPressed(VK_RIGHT))
		cam.MoveBy( { 5.0f, 0.0f });
	if (wnd.IsKeyPressed(VK_DOWN))
		cam.MoveBy( { 0.0f, 5.0f });

}

void BossCharlestonScene::OnKeyDown(BYTE keyCode)
{
	switch (keyCode)
	{
		case VK_SPACE:
			curMap = curMap == mapDark.get() ? mapLight.get() : mapDark.get();
			break;

		case VK_RETURN:
			DoTransitionScene(Scene::Pittsburgh);
			break;
	}
}
