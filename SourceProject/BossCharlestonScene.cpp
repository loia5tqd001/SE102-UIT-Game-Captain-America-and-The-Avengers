#include "pch.h"
#include "BossCharlestonScene.h"
#include "BulletEnemyWizard.h"

static auto& cam = Camera::Instance();

void BossCharlestonScene::Beginning(float dt)
{
	static int checkSpawnOnce = 4;
	if (checkSpawnOnce == 0 && wizard == nullptr) {
		checkSpawnOnce = 4;
		counterBegin = 0.0f;
		counterEnd = 0.0f;
	}
	if (counterBegin > 13.0f) return;
	counterBegin += dt;
	if (counterBegin > 12.0f)
	{
		if (checkSpawnOnce == 1) {
			wizard = std::make_shared<EnemyWizard>(Vector2{ 220.0f, 0.0f }, Vector2{ 0 , 0 }, -1, grid.get(), *cap.get());
			grid->SpawnObject(wizard);
			checkSpawnOnce--;
		}
	}
	else if (counterBegin > 9.0f)
	{
		if (checkSpawnOnce == 2) {
			grid->SpawnObject(std::make_unique<BulletEnemyWizard>(-1, Vector2{ 230,160 }, Vector2{ -BulletEnemyWizard::GetXSpeed(), 0.0f }, nullptr));
			checkSpawnOnce--;
		}
	}
	else if (counterBegin > 6.0f)
	{
		if (checkSpawnOnce == 3) {
			grid->SpawnObject(std::make_unique<BulletEnemyWizard>(1, Vector2{ 0,160 }, Vector2{ BulletEnemyWizard::GetXSpeed(), 0.0f }, nullptr));
			checkSpawnOnce--;
		}
	}
	else if (counterBegin > 3.0f)
	{
		if (checkSpawnOnce == 4) {
			grid->SpawnObject(std::make_unique<BulletEnemyWizard>(-1, Vector2{ 240,160 }, Vector2{ -BulletEnemyWizard::GetXSpeed(),0.0f }, nullptr));
			checkSpawnOnce--;
		}
	}
}

void BossCharlestonScene::Ending(float dt)
{
	if (!wizard) return;
	if (wizard->isDefeated()) {
		counterEnd += dt;
		if (counterEnd > 3.0f) {
			SceneManager::Instance().GoNextScene();
		}
	}
}

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
}

void BossCharlestonScene::Update(float dt)
{
	grid->UpdateCells();
	for (auto& obj : grid->GetObjectsInViewPort())
		obj->Update(dt);
	cap->Update(dt, grid->GetObjectsInViewPort());
	cap->ClampWithin( mapDark->GetWorldBoundary().Trim(16.0f, 15.0f, 16.0f, 43.0f) );
	cam.ClampWithin( mapDark->GetWorldBoundary() );

	Beginning(dt);
	Ending(dt);
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
		case VK_TAB:
			ToggleLight();
			break;

		case VK_RETURN:
			DoTransitionScene(Scene::Pittsburgh);
			break;
	}
	cap->OnKeyDown(keyCode);
}

