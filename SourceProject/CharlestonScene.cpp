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
#include "EnemyWizard.h"


static auto& cam = Camera::Instance();
static auto& wnd = Window::Instance();


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
	grid->UpdateCells();
	for (auto& obj : grid->GetObjectsInViewPort())
	{
		obj->Update(dt);
	}
	//cam.CenterTo( cap->GetBBox().GetCenter() );
	cam.ClampWithin( map->GetWorldBoundary() );

	cap->Update(dt, grid->GetObjectsInViewPort());

	grid->RemoveDestroyedObjects();
}

void CharlestonScene::Draw()
{	
	map->Render();
	cap->Render();

	for (auto& obj : grid->GetObjectsInViewPort())
	{
		obj->Render();
	}

	grid->RenderCells();
	

	#pragma region _TESTING_

	if (1) // test EnemyWizard
	{
	    Data data;
		data.Add("water-velocity", 16.9f);
		data.Add("damage", 420);
		static EnemyWizard enemyWizard(Behaviors::EnemyRocket_ShootCross, data,{ 50.0f, 150.0f }, {}, 1, grid.get(), cap.get());
		std::vector<GameObject*> co;
		enemyWizard.Update(GameTimer::Dt(), co);

		enemyWizard.testing(wnd);

		enemyWizard.Render();
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
