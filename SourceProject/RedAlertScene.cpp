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
}

void RedAlertScene::Update(float dt)
{
	cam.ClampWithin( map->GetWorldBoundary() );
}

void RedAlertScene::Draw()
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

void RedAlertScene::OnKeyDown(BYTE keyCode)
{
	switch (keyCode)
	{
		case VK_RETURN:
			SceneManager::Instance().SetScene( Scene::Charleston );
			break;
	}
}

