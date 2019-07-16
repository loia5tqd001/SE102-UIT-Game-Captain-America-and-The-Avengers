#include "pch.h"
#include "CharlestonScene.h"

static auto& cam = Camera::Instance();

CharlestonScene::CharlestonScene()
{
	LoadResources();

	if (!IsPause()) 
		Sounds::PlayLoop( GetBgMusic() );
}

void CharlestonScene::LoadResources()
{
	const auto root = GetRootJson("Resources\\Data\\scene-charleston.json");

	map = std::make_unique<Map>( root );
}

void CharlestonScene::Update(float dt)
{
	cam.ClampWithin( map->GetWorldBoundary() );
}
#include "EnemyGun.h"
#include "BulletEnemyGun.h"
#include "EnemyRocket.h"

void CharlestonScene::Draw()
{
	static auto& wnd = Window::Instance();
	map->Render();

	if (1) // test enemyGun
	{
		static EnemyGun enemyGun( { 100.0f, 100.0f }, {}, 1, nullptr );
		std::vector<GameObject*> co;
		enemyGun.Update(GameTimer::Dt(), co);

		if (wnd.IsKeyPressed('1')) enemyGun.SetState(State::EnemyGun_Stand);
		if (wnd.IsKeyPressed('2')) enemyGun.SetState(State::EnemyGun_Sitting);
		if (wnd.IsKeyPressed('3')) enemyGun.TakeDamage(1);
		if (wnd.IsKeyPressed('4')) enemyGun.SetState(State::EnemyGun_Walking);

		enemyGun.Render();
	}

	if (1) // test BulletEnemyGun
	{
		static BulletEnemyGun BulletEnemyGun( { 100.0f, 150.0f },  {}, 1 );
		std::vector<GameObject*> co;
		BulletEnemyGun.Update(GameTimer::Dt(), co);

		if (wnd.IsKeyPressed('5')) BulletEnemyGun.SetState(State::BulletEnemyGun);

		BulletEnemyGun.Render();
	}

	if (1) // test EnemyRocket
	{
		static EnemyRocket enemyRocket( { 150.0f, 150.0f }, {}, 1, nullptr );
		std::vector<GameObject*> co;
		enemyRocket.Update(GameTimer::Dt(), co);

		if (wnd.IsKeyPressed('6')) enemyRocket.SetState(State::EnemyRocket_Stand);
		if (wnd.IsKeyPressed('7')) enemyRocket.SetState(State::EnemyRocket_Sitting);
		if (wnd.IsKeyPressed('8')) enemyRocket.SetState(State::EnemyRocket_TakeDamage);
		if (wnd.IsKeyPressed('9')) enemyRocket.SetState(State::EnemyRocket_Walking);

		enemyRocket.Render();
	}

	if (wnd.IsKeyPressed(VK_LEFT))
		cam.MoveBy( { -5.0f, 0.0f });
	if (wnd.IsKeyPressed(VK_UP))
		cam.MoveBy( { 0.0f, -5.0f });
	if (wnd.IsKeyPressed(VK_RIGHT))
		cam.MoveBy( { 5.0f, 0.0f });
	if (wnd.IsKeyPressed(VK_DOWN))
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
}
