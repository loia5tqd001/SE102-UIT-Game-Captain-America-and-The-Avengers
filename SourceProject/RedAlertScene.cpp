#include "pch.h"
#include "RedAlertScene.h"

static auto& cam = Camera::Instance();

RedAlertScene::RedAlertScene()
{
	LoadResources();

	if (!IsPause())
		Sounds::PlayLoop(GetBgMusic());
}

void RedAlertScene::LoadResources()
{
	const auto root = GetRootJson("Resources\\Data\\scene-red-alert.json");

	map = std::make_unique<Map>(root);
	grid = std::make_unique<Grid>(root);
	cap = std::make_unique<Captain>(Vector2{ 32.0f, 196.0f - 45.0f }, grid.get());
	grid->SpawnObject(std::make_unique<DynamiteNapalm>(Behaviors::DynamiteNapalm_Fall, Data{}, Vector2{ 180,10 }, Vector2{ 0,0 }, -1, grid.get(), *cap.get()));
}



void RedAlertScene::Update(float dt)
{
	bossIsDestroyed = true;
	map->UpdateAnimatedTiles(dt);
	grid->UpdateCells();
	for (auto& obj : grid->GetObjectsInViewPort())
	{
		if (bossIsDestroyed&&dynamic_cast<DynamiteNapalm*>(obj))
		{
			bossIsDestroyed = false;
		}
		obj->Update(dt, grid->GetObjectsInViewPort());
	}
	if (bossIsDestroyed)
	{
		SceneManager::Instance().GoNextScene();
	}
	cap->Update(dt, grid->GetObjectsInViewPort());
	cap->ClampWithin(map->GetWorldBoundary().Trim(16.0f, 0.0f, 16.0f, 44.0f));
	cam.ClampWithin(map->GetWorldBoundary());
}

void RedAlertScene::Draw()
{
	map->Render();
	for (auto& obj : grid->GetObjectsInViewPort())
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

