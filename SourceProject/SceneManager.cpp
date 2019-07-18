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

void SceneManager::LoadResources()
{
	const auto root = AbstractScene::GetRootJson("Resources\\Data\\resources.json");

	Textures     ::LoadResources(root);
	Sprites      ::LoadResources(root);
	Sounds       ::LoadResources(root);
	KeyCodeFonts ::LoadResources(root);
	DebugDraw    ::InitDefaultFont();
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
		ExitSign::Instance().Draw();
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
