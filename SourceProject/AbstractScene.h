#pragma once

class AbstractScene
{
private:
	bool isPause = Settings::Instance().IsOpening();
	bool isDoingTransitionScene = false;
	std::optional<Scene> nextScene;

public:
	AbstractScene() = default;
	AbstractScene(const AbstractScene&) = delete;
	virtual ~AbstractScene() = default;

	static Json::Value GetRootJson(LPCSTR jsonPath);

	virtual SoundId GetBgMusic() const { return SoundId::Invalid; } 
	bool HasMusic() const { return GetBgMusic() != SoundId::Invalid; }

	void SetPause(bool ispause);
	void TogglePause() { SetPause(!isPause); }
	inline bool IsPause() const { return isPause; }

	virtual void SetCapPos(Vector2 pos) {}
	virtual void Teleport() {} // in pittsburgh, between rooms and main scene.

	void DoTransitionScene(std::optional<Scene> nextScene = {}, float transTime = 1.23f);
	inline bool IsDoingTransitionSceneEffect() const { return isDoingTransitionScene; }
	std::optional<Scene> GetNextScene() const;

	bool canGoNextMap = false;
	bool isDark = false; 
	void ToggleLight();

	virtual void LoadResources () = 0;
	virtual void Update(float dt) = 0;
	virtual void Draw  (        ) = 0;

	virtual void OnKeyDown(BYTE keyCode) {}
	virtual void OnKeyUp  (BYTE keyCode) {}
};
