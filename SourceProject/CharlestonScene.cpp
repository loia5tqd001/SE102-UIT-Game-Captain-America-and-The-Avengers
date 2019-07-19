#include "pch.h"
#include "CharlestonScene.h"

#include "EnemyGun.h"
#include "BulletEnemyGun.h"
#include "EnemyRocket.h"
#include "BulletEnemyRocket.h"

#include "Capsule.h"
#include "Item.h"
#include "Captain.h"
#include "CaptainHealth.h"
#include "Shield.h"
#include "Spawner.h"
static auto& cam = Camera::Instance();

CharlestonScene::CharlestonScene()
{
	LoadResources();
	
	if (!IsPause())
	{
		Sounds::PlayLoop( GetBgMusic() );
	}
	
}

void CharlestonScene::LoadResources()
{
	const auto root = GetRootJson("Resources\\Data\\scene-charleston.json");

	map = std::make_unique<Map>( root );
	grid = std::make_unique<Grid>( root );
	cap = std::make_unique<Captain>(Vector2(0, 100));
}

void CharlestonScene::Update(float dt)
{


	static Window& wnd = Window::Instance();
	grid->UpdateCells();
	if (1)
	{
		static Spawner spawner({256.0f,0.0f}, 16, 480, Behaviors::EnemyGun_Shoot, { 73.0f, 404.0f }, Data{}, grid.get());
		if (wnd.IsKeyPressed(VK_NUMPAD0))
			spawner.OnCollideWithCap();
		spawner.Update( GameTimer::Dt() );
	}
	for (auto& obj : grid->GetObjectsInViewPort())
	{
		obj->Update(dt);
	}
	cam.ClampWithin( map->GetWorldBoundary() );
	std::vector<GameObject*> co;
	cap->Update(dt, co);
	grid->RemoveDestroyedObjects();
}
#include "EnemyWizard.h"
void CharlestonScene::Draw()
{	

	static auto& wnd = Window::Instance();
	map->Render();
	cap->Render();

	#pragma region _TESTING_

	if (1) // test EnemyWizard
	{
	    Data data;
		data.Add("water-velocity", 16.9f);
		data.Add("damage", 420);
		static EnemyWizard enemyWizard(Behaviors::EnemyGun_Shoot, data,{ 50.0f, 150.0f }, {}, 1, nullptr);
		std::vector<GameObject*> co;
		enemyWizard.Update(GameTimer::Dt(), co);

		enemyWizard.testing(wnd);

		enemyWizard.Render();
	}

	// Test Capsule, NOTE: Can't test because there's not grid yet
	if (1)
	{
		static Capsule capsule( { 0.0f, 10.0f}, SpriteId::ItemKeyKrystal, 100.0f, grid.get());
		if (wnd.IsKeyPressed(VK_NUMPAD9))
		{
			capsule.BeingHit();
		}
		capsule.Update( GameTimer::Dt() );
		capsule.Render();
	}

	

	//if (0)
	//{
	//	static Captain cap(Vector2(0, 100));
	//	std::vector<GameObject*> co;
	//	cap.Update(GameTimer::Dt(), co);

	//	cap.Render();

	//	//test shield
	//	static Captain *captain = &cap;
	//	static Shield shield(captain);
	//	shield.Update(GameTimer::Dt(), co);
	//	static auto& setting = Settings::Instance();
	//	if (wnd.IsKeyPressed('T'))
	//		shield.ThrowAway();

	//	shield.Render();
	//}

	if (1)
	{
		if (wnd.IsKeyPressed('O')) CaptainHealth::Instance().Add(1);
		if (wnd.IsKeyPressed('P')) CaptainHealth::Instance().Subtract(1);
		if (wnd.IsKeyPressed('0')) CaptainHealth::Instance().Set(0);
	}
#pragma endregion

	if (wnd.IsKeyPressed('A'))
		cam.MoveBy( { -5.0f, 0.0f });
	if (wnd.IsKeyPressed('W'))
		cam.MoveBy( { 0.0f, -5.0f });
	if (wnd.IsKeyPressed('D'))
		cam.MoveBy( { 5.0f, 0.0f });
	if (wnd.IsKeyPressed('S'))
		cam.MoveBy( { 0.0f, 5.0f });
	for (auto& obj : grid->GetObjectsInViewPort())
	{
		obj->Render();
	}
	grid->RenderCells();

}

void CharlestonScene::OnKeyDown(BYTE keyCode)
{
	switch (keyCode)
	{
		case VK_RETURN:
			DoTransitionScene(Scene::BossCharleston);
			break;

	}
	cap->OnKeyDown(keyCode);
}
