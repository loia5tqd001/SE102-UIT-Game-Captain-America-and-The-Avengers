#pragma once

struct CheckPoint
{
	Scene scene;
	Vector2 captainPos = {};
	bool canGoNextMap = false;
};

class SceneManager
{
private:
	std::unique_ptr<AbstractScene> curScene;
	Settings& settingScene = Settings::Instance();
	CheckPoint checkpoints[8];

public:
	void ToggleMuteMode() const;
	void ToggleSettingScene() const;

	void GoToCheckPoint(CheckPoint checkpoint);
	void LoadResources();
	void SetScene(Scene scene);
	void GoNextScene();
	void Update(float dt);
	void Draw();

	void OnKeyDown(BYTE keyCode);
	void OnKeyUp(BYTE keyCode);

	inline AbstractScene& GetCurScene() { return *curScene.get(); }

private:
	SceneManager() = default;
	SceneManager(const SceneManager&) = delete;

public:
	static SceneManager& Instance();
};

