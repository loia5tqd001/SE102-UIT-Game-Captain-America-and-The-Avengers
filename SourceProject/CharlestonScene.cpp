#include "pch.h"
#include "CharlestonScene.h"
#include "EnemyGun.h"
#include "BulletEnemyGun.h"
#include "EnemyRocket.h"
#include "BulletEnemyRocket.h"

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
	cap = std::make_unique<Captain>( Vector2{ 14.0f, 391.0f }, grid.get() ) ;
	cam.SetMainCharacter(cap.get());
}

void CharlestonScene::Update(float dt)
{
	grid->UpdateCells(); 
	map->UpdateAnimatedTiles(dt);

	for (auto& obj : grid->GetObjectsInViewPort()) // update objects
		obj->Update(dt);
	
	cap->Update(dt, grid->GetObjectsInViewPort()); // update Captain

	// clamp captain and camera
	cap->ClampWithin(map->GetWorldBoundary().Trim(14.0f, 0.0f, 14.0f, 0.0f));
	cam.FollowMainCharacter();
	cam.ClampWithin(map->GetWorldBoundary());

}

void CharlestonScene::Draw()
{	
	map->Render(); // layer0
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

void CharlestonScene::SetCapPos(Vector2 pos)
{
	cap->SetPos(pos);
}

