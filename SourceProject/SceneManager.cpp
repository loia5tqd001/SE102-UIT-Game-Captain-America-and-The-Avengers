#include "pch.h"
#include "GreetingScene.h"
#include "CharlestonScene.h"
#include "BossCharlestonScene.h"
#include "PittsburghScene.h"
#include "RedAlertScene.h"
#include "ExitSign.h"
#include "CaptainHealth.h"

void SceneManager::ToggleMuteMode() const
{
	Sounds::SetMute( !Sounds::IsMute() );

	if (!Sounds::IsMute() && !curScene->IsPause() && curScene->HasMusic())
		Sounds::PlayLoop(curScene->GetBgMusic());
}

void SceneManager::ToggleSettingScene() const
{
	curScene->TogglePause();
	settingScene.SetOpening( curScene->IsPause() );
}

void SceneManager::GoToCheckPoint(CheckPoint checkpoint)
{
	SetScene(checkpoint.scene);
	if (checkpoint.captainPos != Vector2{}) 
		curScene->SetCapPos(checkpoint.captainPos);
	if (checkpoint.canGoNextMap)
	{
		curScene->canGoNextMap = true;
		ExitSign::Instance().KrystalCollected( {} );
	}
}

void SceneManager::LoadResources()
{
	const auto root = AbstractScene::GetRootJson("Resources\\Data\\resources.json");

	Textures     ::LoadResources(root);
	Sprites      ::LoadResources(root);
	Sounds       ::LoadResources(root);
	KeyCodeFonts ::LoadResources(root);
	DebugDraw    ::InitDefaultFont();

	checkpoints[0] = CheckPoint{ Scene::Charleston };
	checkpoints[1] = CheckPoint{ Scene::Charleston, Vector2{ 1700.0f, 391.0f }, true };
	checkpoints[2] = CheckPoint{ Scene::BossCharleston };
	checkpoints[3] = CheckPoint{ Scene::Pittsburgh };
	checkpoints[4] = CheckPoint{ Scene::Pittsburgh, Vector2{ 816.0f, 628.0f } };
	checkpoints[5] = CheckPoint{ Scene::Pittsburgh, Vector2{ 560.0f, 148.0f } };
	checkpoints[6] = CheckPoint{ Scene::Pittsburgh, Vector2{ 720.0f, 852.0f }, true };
	checkpoints[7] = CheckPoint{ Scene::RedAlert };
}

void SceneManager::SetScene(Scene scene)
{
	if (curScene && curScene->HasMusic())
	{
		Sounds::StopAt( curScene->GetBgMusic() );
		ExitSign::Instance().ResetNewStage();
	}

	switch (scene)
	{
		case Scene::Greeting:
			curScene = std::make_unique<GreetingScene>();
			break;

		case Scene::Charleston:
			curScene = std::make_unique<CharlestonScene>();
			break;

		case Scene::BossCharleston:
			curScene = std::make_unique<BossCharlestonScene>();
			break;

		case Scene::Pittsburgh:
			curScene = std::make_unique<PittsburghScene>();
			break;

		case Scene::RedAlert:
			curScene = std::make_unique<RedAlertScene>();
			break;

		default:
			ThrowMyException("Failed to change to unknown scene");
	}
}

void SceneManager::GoNextScene()
{
	if (dynamic_cast< GreetingScene*>(curScene.get())) curScene->DoTransitionScene(Scene::Charleston);
	if (dynamic_cast< CharlestonScene*>(curScene.get())) curScene->DoTransitionScene(Scene::BossCharleston);
	if (dynamic_cast< BossCharlestonScene*>(curScene.get())) curScene->DoTransitionScene(Scene::Pittsburgh);
	if (dynamic_cast< PittsburghScene*>(curScene.get())) curScene->DoTransitionScene(Scene::RedAlert);
	if (dynamic_cast< RedAlertScene*>(curScene.get())) curScene->DoTransitionScene(Scene::Greeting);
}

void SceneManager::Update(float dt)
{
	if (!curScene->IsPause())
	{
		curScene->Update(dt);
	}

	Sounds::HandleInput();

	if (curScene->IsDoingTransitionSceneEffect()) {
		curScene->DoTransitionScene(); // update transition scene, not "do", "update"
	}
	if (curScene->GetNextScene()) {
		SetScene(*curScene->GetNextScene());
	}
}

void SceneManager::Draw()
{
	if (curScene->IsDoingTransitionSceneEffect()) {
		Game::Instance().FillColor(Colors::Black);
	}
	else {
		curScene->Draw();
		CaptainHealth::Instance().Draw();
		DebugDraw::DrawCrt();
	}
	settingScene.Draw();
	Sounds::Draw();
}

void SceneManager::OnKeyDown(BYTE keyCode)
{
	if (settingScene.IsOpening()) 
		settingScene.OnKeyDown(keyCode);
	else                         
 		curScene->OnKeyDown(keyCode);

	switch (keyCode)
	{
		case VK_ESCAPE:
			if (!settingScene.IsEditing())
				ToggleSettingScene();
			break;

		case VK_MENU:
			DebugDraw::ToggleCrtMode();
			break;

		case VK_CONTROL:
			DebugDraw::ToggleDebugMode();
			break;

		case VK_ADD:
			CaptainHealth::Instance().Add(1);
			break;

		case VK_SUBTRACT:
			if (CaptainHealth::Instance().Get() > 1)
				CaptainHealth::Instance().Subtract(1);
			break;

		case VK_NUMPAD0: GoToCheckPoint(checkpoints[0]); break;
		case VK_NUMPAD1: GoToCheckPoint(checkpoints[1]); break;
		case VK_NUMPAD2: GoToCheckPoint(checkpoints[2]); break;
		case VK_NUMPAD3: GoToCheckPoint(checkpoints[3]); break;
		case VK_NUMPAD4: GoToCheckPoint(checkpoints[4]); break;
		case VK_NUMPAD5: GoToCheckPoint(checkpoints[5]); break;
		case VK_NUMPAD6: GoToCheckPoint(checkpoints[6]); break;
		case VK_NUMPAD7: GoToCheckPoint(checkpoints[7]); break;

		case 'M':
			ToggleMuteMode();
			break;		
	}	
}

void SceneManager::OnKeyUp(BYTE keyCode)
{
	if (!curScene->IsPause())
		curScene->OnKeyUp(keyCode);
}

SceneManager& SceneManager::Instance()
{
	static SceneManager instance;
	return instance;
}
