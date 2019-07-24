#include "pch.h"
#include "CharlestonScene.h"
#include "EnemyGun.h"
#include "BulletEnemyGun.h"
#include "EnemyRocket.h"
#include "BulletEnemyRocket.h"
#include "EnemyFly.h"
#include "EnemyWizard.h"

static auto& cam = Camera::Instance();
static auto& wnd = Window::Instance();
static auto& ambush = AmbushTrigger::Instance();

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
	//cap = std::make_unique<Captain>( Vector2{ 173.0f, 391.0f } ) ;
	cap = std::make_unique<Captain>( Vector2{ 1673.0f, 391.0f } ) ;
	//cap = std::make_unique<Captain>(Vector2{ 53.0f, 20.0f });
}

void CharlestonScene::Update(float dt)
{
	grid->UpdateCells(); 

	for (auto& obj : grid->GetObjectsInViewPort()) // update objects
		obj->Update(dt);

	cap->Update(dt, grid->GetObjectsInViewPort()); // update Captain

	// clamping 
	// clamping cap
	// clamping camera

	if (ambush->GetState() == State::Ambush_Being) 
		cap->ClampWithin(ambush->GetLockCaptain());
	else 
		cap->ClampWithin( map->GetWorldBoundary().Trim(14.0f, 0.0f, 14.0f, 0.0f) ); 

	static bool shouldCenterCap = true;
	if (ambush->GetState() == State::Ambush_Being) {
		cam.ClampWithin(ambush->GetLockCamera());
		shouldCenterCap = false;
	}
	else {
		if (std::abs(cam.GetBBox().GetCenter().x - cap->GetCenter().x) <= 14.0f) {
			cam.SetRadius(14.0f);
		}
		
		cam.CenterAround( cap->GetCenter() );
		cam.ClampWithin( map->GetWorldBoundary() );
	}



	if (wnd.IsKeyPressed(VK_NUMPAD5))
	{
		cap->SetState(State::Captain_FallToWater);
	}

	if (wnd.IsKeyPressed(VK_NUMPAD6))
	{
		cap->SetState(State::Captain_Climbing);
	}

	if (wnd.IsKeyPressed(VK_NUMPAD7))
	{
		cap->SetState(State::Captain_Falling);
	}

	if (wnd.IsKeyPressed(VK_NUMPAD8))
	{
		cap->SetState(State::Captain_Injured);
	}
}

void CharlestonScene::Draw()
{	
	map->Render(); // layer0

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
	

	#pragma region _TESTING_

	if (0) // test EnemyWizard
	{
	    Data data;
		data.Add("water-velocity", 16.9f);
		data.Add("damage", 420);
		static EnemyWizard enemyWizard(Behaviors::EnemyRocket_ShootCross, std::move(data),{ 150.0f, 250.0f }, {}, 1, grid.get(), *cap.get());
		std::vector<GameObject*> co;
		enemyWizard.Update(GameTimer::Dt(), co);
		enemyWizard.testing(wnd);

		enemyWizard.Render();
	}
	if (0) // test EnemyFly
	{
		Data data;
		data.Add("water-velocity", 16.9f);
		data.Add("damage", 420);
		static EnemyFly enemyFly({ 50.0f, 260.0f }, grid.get(), cap.get());
		std::vector<GameObject*> co;
		enemyFly.Update(GameTimer::Dt(), co);

		enemyFly.Render();
	}
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

void CharlestonScene::OnKeyUp(BYTE keyCode)
{
	switch (keyCode)
	{
	default:
		break;
	}
	cap->OnKeyUp(keyCode);
}

