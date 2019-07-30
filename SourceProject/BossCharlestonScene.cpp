#include "pch.h"
#include "BossCharlestonScene.h"
#include "EnemyWizard.h"

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
	cap = std::make_unique<Captain>( Vector2{ 32.0f, 197.0f - 45.0f },grid.get()) ;
	grid->SpawnObject(std::make_unique<EnemyWizard>(Vector2{ 31.0f, 9.0f }, Vector2{0 , 0}, 1, grid.get(), *cap.get())) ;
	//wizard = new EnemyWizard(Vector2{ 31.0f, 9.0f }, Vector2{ 0 , 0 }, 1, grid.get(), *cap.get());
}

void BossCharlestonScene::Update(float dt)
{
	grid->UpdateCells();
	for (auto& obj : grid->GetObjectsInViewPort())
		obj->Update(dt);
	cap->Update(dt, grid->GetObjectsInViewPort());
	cap->ClampWithin( mapDark->GetWorldBoundary().Trim(16.0f, 15.0f, 16.0f, 43.0f) );
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

#pragma region testing
	//if (1) // test EnemyWizard
	//{
	//	Data data;
	//	data.Add("water-velocity", 16.9f);
	//	data.Add("damage", 420);
	//	static EnemyWizard enemyWizard( { 31.0f, 9.0f }, {}, 1, grid.get(), *cap.get());
	//	std::vector<GameObject*> co;
	//	enemyWizard.Update(GameTimer::Dt(), co);

	//	enemyWizard.Render();
	//}
#pragma endregion

}

void BossCharlestonScene::OnKeyUp(BYTE keyCode)
{
	cap->OnKeyUp(keyCode);
}

void BossCharlestonScene::OnKeyDown(BYTE keyCode)
{
	switch (keyCode)
	{
		case VK_TAB:
			ToggleLight();
			break;

		case VK_RETURN:
			DoTransitionScene(Scene::Pittsburgh);
			break;
	}
	cap->OnKeyDown(keyCode);
}

