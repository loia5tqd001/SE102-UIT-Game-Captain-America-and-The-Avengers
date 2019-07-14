#include "pch.h"
#include "PittsburghScene.h"

static auto& cam = Camera::Instance();

PittsburghScene::PittsburghScene()
{
	LoadResources();

	if (!IsPause()) 
		Sounds::PlayLoop( GetBgMusic() );
}

void PittsburghScene::LoadResources()
{
	const auto root = GetRootJson("Resources\\Data\\scene-pittsburgh.json");

	mapDark = std::make_unique<Map>( root["dark"] );
	mapLight = std::make_unique<Map>( root["light"] );
	curMap = mapDark.get();
}

void PittsburghScene::Update(float dt)
{
	cam.ClampWithin( curMap->GetWorldBoundary() );
}

void PittsburghScene::Draw()
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

void PittsburghScene::OnKeyDown(BYTE keyCode)
{
	switch (keyCode)
	{
		case VK_RETURN:
			curMap = curMap == mapDark.get() ? mapLight.get() : mapDark.get();
			break;
	}
}
