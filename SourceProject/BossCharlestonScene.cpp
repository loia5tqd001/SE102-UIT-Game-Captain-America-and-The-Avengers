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

	map = std::make_unique<Map>( root );
}

void BossCharlestonScene::Update(float dt)
{
	cam.ClampWithin( map->GetWorldBoundary() );
}

void BossCharlestonScene::Draw()
{
	map->Render();

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
		case VK_CONTROL:
			DebugDraw::ToggleDebugMode();
			break;
	}
}
