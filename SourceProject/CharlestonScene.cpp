#include "pch.h"
#include "CharlestonScene.h"


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
	cap = std::make_unique<Captain>(Vector2(0, 100));
}

void CharlestonScene::Update(float dt)
{
	cam.ClampWithin( map->GetWorldBoundary() );
	std::vector<GameObject*> co;
	cap->Update(dt, co);
}
#include "EnemyGun.h"
#include "BulletEnemyGun.h"
#include "EnemyRocket.h"
#include "BulletEnemyRocket.h"

#include "Capsule.h"
#include "Item.h"
#include "Captain.h"
#include "CaptainHealth.h"
#include "Shield.h"

void CharlestonScene::Draw()
{
	static auto& wnd = Window::Instance();
	map->Render();
	cap->Render();
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
		static BulletEnemyGun BulletEnemyGun(  1, {20.0f, 150.0f } );
		std::vector<GameObject*> co;
		BulletEnemyGun.Update(GameTimer::Dt(), co);

		BulletEnemyGun.SetState(State::BulletEnemyGun);

		BulletEnemyGun.Render();
	}

	if (1) // test BulletEnemyRocket
	{
		static BulletEnemyRocket BulletEnemyRocket(  1, 1, {20.0f, 170.0f }); //1 is cross, 0 is horizontal
		std::vector<GameObject*> co;
		BulletEnemyRocket.Update(GameTimer::Dt(), co);

		BulletEnemyRocket.Render();
	}

	if (1) // test EnemyRocket
	{
		static EnemyRocket enemyRocket( { 150.0f, 150.0f }, {}, 1, nullptr );
		std::vector<GameObject*> co;
		enemyRocket.Update(GameTimer::Dt(), co);

		if (wnd.IsKeyPressed('6')) enemyRocket.SetState(State::EnemyRocket_Stand);
		if (wnd.IsKeyPressed('7')) enemyRocket.SetState(State::EnemyRocket_Sitting);
		if (wnd.IsKeyPressed('8')) 
			enemyRocket.TakeDamage(1);
		if (wnd.IsKeyPressed('9')) enemyRocket.SetState(State::EnemyRocket_Walking);

		enemyRocket.Render();
	}

	// Test Capsule, NOTE: Can't test because there's not grid yet
	if (1)
	{
		static Capsule capsule( { 0.0f, 10.0f}, SpriteId::ItemKeyKrystal, 100.0f, nullptr);
		if (wnd.IsKeyPressed(VK_NUMPAD9))
		{
			capsule.BeingHit();
		}
		capsule.Update( GameTimer::Dt() );
		capsule.Render();
	}

	//Test Items
	if (1)
	{

		static std::unique_ptr<Item> item = std::make_unique<Item> ( Vector2{ 20.0f, 20.0f }, 60.0f, SpriteId::ItemKeyKrystal );
		
		if (wnd.IsKeyPressed(VK_NUMPAD0)) item = std::make_unique<Item>( Vector2{ 20.0f, 20.0f }, 60.0f, SpriteId::ItemSmallPowerStone );
		if (wnd.IsKeyPressed(VK_NUMPAD1)) item = std::make_unique<Item>( Vector2{ 20.0f, 20.0f }, 60.0f, SpriteId::ItemBigPowerStone   );
		if (wnd.IsKeyPressed(VK_NUMPAD2)) item = std::make_unique<Item>( Vector2{ 20.0f, 20.0f }, 60.0f, SpriteId::ItemFivePoint       );
		if (wnd.IsKeyPressed(VK_NUMPAD3)) item = std::make_unique<Item>( Vector2{ 20.0f, 20.0f }, 60.0f, SpriteId::ItemOneUp           );
		if (wnd.IsKeyPressed(VK_NUMPAD4)) item = std::make_unique<Item>( Vector2{ 20.0f, 20.0f }, 60.0f, SpriteId::ItemSmallEnergy     );
		if (wnd.IsKeyPressed(VK_NUMPAD5)) item = std::make_unique<Item>( Vector2{ 20.0f, 20.0f }, 60.0f, SpriteId::ItemBigEnergy       );
		if (wnd.IsKeyPressed(VK_NUMPAD6)) item = std::make_unique<Item>( Vector2{ 20.0f, 20.0f }, 60.0f, SpriteId::ItemKeyKrystal      );
		
		item->Update( GameTimer::Dt() );

		if (wnd.IsKeyPressed('C'))
			item->BeingHit();
		if (wnd.IsKeyPressed('Z'))
			item->BeingCollected();
		item->Render();
	}

	/*if (1)
	{
		static Captain cap(Vector2(0, 100));
		std::vector<GameObject*> co;
		cap.Update(GameTimer::Dt(), co);

		cap.Render();

		//test shield
		static Captain *captain = &cap;
		static Shield shield(captain);
		shield.Update(GameTimer::Dt(), co);
		static auto& setting = Settings::Instance();
		if (wnd.IsKeyPressed('T'))
			shield.ThrowAway();

		shield.Render();
	}

	if (1)
	{
		if (wnd.IsKeyPressed('O')) CaptainHealth::Instance().Add(1);
		if (wnd.IsKeyPressed('P')) CaptainHealth::Instance().Subtract(1);
		if (wnd.IsKeyPressed('0')) CaptainHealth::Instance().Set(0);
	}
	}*/
	/*if (wnd.IsKeyPressed(VK_LEFT))
		cam.MoveBy( { -5.0f, 0.0f });
	if (wnd.IsKeyPressed(VK_UP))
		cam.MoveBy( { 0.0f, -5.0f });
	if (wnd.IsKeyPressed(VK_RIGHT))
		cam.MoveBy( { 5.0f, 0.0f });
	if (wnd.IsKeyPressed(VK_DOWN))
		cam.MoveBy( { 0.0f, 5.0f });*/
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
