#include "pch.h"
#include "PittsburghScene.h"

static auto& cam = Camera::Instance();
static auto& wnd = Window::Instance();

PittsburghScene::PittsburghScene()
{
	LoadResources();
	isDark = true;
	if (!IsPause()) 
		Sounds::PlayLoop( GetBgMusic() );
}

void PittsburghScene::LoadResources()
{
	const auto root = GetRootJson("Resources\\Data\\scene-pittsburgh.json");

	mapDark = std::make_unique<Map>( root["dark"] );
	mapLight = std::make_unique<Map>( root["light"] );

	grid = std::make_unique<Grid>( root );
	cap = std::make_unique<Captain>( Vector2{ 32.0f, 197.0f - 45.0f },grid.get()) ;

	cam.SetMainCharacter(cap.get());
	ClampCaptainAndCamera();
}

void PittsburghScene::Update(float dt)
{
	grid->UpdateCells(); 

	for (auto& obj : grid->GetObjectsInViewPort()) // update objects
		obj->Update(dt, grid->GetObjectsInViewPort());

	cap->Update(dt, grid->GetObjectsInViewPort()); // update Captain

	ClampCaptainAndCamera();
}

void PittsburghScene::Draw()
{
	if (isDark) mapDark->Render();
	else mapLight->Render();

	ExitSign::Instance().Draw();

	// layer1: capsules
	std::vector<GameObject*> layer2; // item, bullet, enemy, ledge (other visible objects)
									 // layer3: captain
	std::vector<GameObject*> layer4; // invisible object

	for (auto& obj : grid->GetObjectsInViewPort()) {
		if (dynamic_cast<Capsule*>(obj)) 
			obj->Render();
		else if (dynamic_cast<VisibleObject*>(obj)) 
			layer2.emplace_back(obj);
		else 
			layer4.emplace_back(obj);
	}

	for (auto& obj : layer2) obj->Render();
	cap->Render();
	for (auto& obj : layer4) obj->Render();

	grid->RenderCells();


#if 0
	if (wnd.IsKeyPressed(VK_LEFT))
		cam.MoveBy( { -5.0f, 0.0f });
	if (wnd.IsKeyPressed(VK_UP))
		cam.MoveBy( { 0.0f, -5.0f });
	if (wnd.IsKeyPressed(VK_RIGHT))
		cam.MoveBy( { 5.0f, 0.0f });
	if (wnd.IsKeyPressed(VK_DOWN))
		cam.MoveBy( { 0.0f, 5.0f });
#endif
}

void PittsburghScene::OnKeyDown(BYTE keyCode)
{
	switch (keyCode)
	{
		case VK_TAB:
			ToggleLight();
			break;

		case VK_RETURN:
			DoTransitionScene(Scene::RedAlert);
			break;
	}
	cap->OnKeyDown(keyCode);
}

void PittsburghScene::OnKeyUp(BYTE keyCode)
{
	cap->OnKeyUp(keyCode);
}

void PittsburghScene::SetCapPos(Vector2 pos)
{
	cap->SetPos(pos);
}

void PittsburghScene::Teleport()
{
	static const auto Room_1 = Vector2{  53.0f, 1128.0f }; // position of captain in those regions
	static const auto Room_2 = Vector2{ 325.0f, 1128.0f };
	static const auto Main_Door1 = Vector2{ 565.0f,  149.0f };
	static const auto Main_Door2 = Vector2{ 821.0f,  632.0f };

	if (currentScenePart == PittsburghScenePart::MainMap)
	{
		if (cap->GetPos().y < 250.0f) // door 1 of main map
		{
			cap->SetPos(Room_1); // if current in door1, then teleport to room1
			currentScenePart = PittsburghScenePart::Room1;
		}
		else // door 2 of main map
		{
			cap->SetPos(Room_2);
			currentScenePart = PittsburghScenePart::Room2;
		}
	}
	else if (currentScenePart == PittsburghScenePart::Room1)
	{
		cap->SetPos(Main_Door1);  // if current in room1, then teleport back to door1
		cam.MoveTo( {} ); // to avoid camera abberance as teleporting
		currentScenePart = PittsburghScenePart::MainMap;
	}
	else if (currentScenePart == PittsburghScenePart::Room2)
	{
		cap->SetPos(Main_Door2);
		cam.MoveTo( {} );
		currentScenePart = PittsburghScenePart::MainMap;
	}
	DoTransitionScene();
}

void PittsburghScene::ClampCaptainAndCamera()
{
	if (currentScenePart == PittsburghScenePart::MainMap)
	{
		cap->ClampWithin( { 16.0f, 16.0f, 1008.0f, 917.0f } ); // clamp region in main map
		cam.FollowMainCharacter();
		cam.ClampWithin( { 0.0f, 0.0f, 1024.0f, 960.0f } ); // world boundary in main map

	}
	else if (currentScenePart == PittsburghScenePart::Room1)
	{
		cap->ClampWithin( { 16.0f, 992.0f, 240.0f, 1173.0f } ); // clamp region in room1
		cam.ClampWithin( { 0.0f, 976.0f, 256.0f, 1216.0f } ); // world boundary in room1
	}
	else if (currentScenePart == PittsburghScenePart::Room2)
	{
		cap->ClampWithin( { 288.0f, 992.0f, 512.0f, 1173.0f } ); // clamp region in room2
		cam.ClampWithin( { 272.0f, 976.0f, 528.0f, 1216.0f } ); // world boundary in room2
	}
}
