#include "pch.h"
#include "PittsburghScene.h"
#include "Bunker.h"

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
	//cap = std::make_unique<Captain>( Vector2{ 792.0f, 390.0f },grid.get()) ;
	cam.SetMainCharacter(cap.get());
}

void PittsburghScene::Update(float dt)
{
	grid->UpdateCells(); 

	for (auto& obj : grid->GetObjectsInViewPort()) // update objects
		obj->Update(dt, grid->GetObjectsInViewPort());

	cap->Update(dt, grid->GetObjectsInViewPort()); // update Captain

	// clamp captain and camera
	cap->ClampWithin(mapDark->GetWorldBoundary().Trim(14.0f, 0.0f, 14.0f, 0.0f));
	cam.FollowMainCharacter();
	cam.ClampWithin(mapDark->GetWorldBoundary());
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

#if 0
	static Bunker bun(State::Bunker_Idle_0, { 792.0f, 390.0f }, grid.get());
	bun.Update(GameTimer::Dt());
	bun.Render();
#endif

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
		case VK_SPACE:
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
