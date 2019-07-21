#pragma once

class Captain;
class CaptainState
{
protected:
	// reference for more convenient reference in subclasses
	Window& wnd = Window::Instance();
	Settings& setting = Settings::Instance();
	SceneManager& sceneManager = SceneManager::Instance();

public:
	virtual void Enter(Captain& cap, State fromState, Data&& data = {}) = 0; // handle when change to this state
	virtual Data Exit(Captain& cap, State toState) = 0; // handle when change to other state
	virtual void OnKeyUp(Captain& cap, BYTE keyCode) {}
	virtual void OnKeyDown(Captain& cap, BYTE keyCode) {}
	virtual void Update(Captain& cap, float dt, const std::vector<GameObject*>& coObjects) = 0; // captain delegates his update call to this function
	virtual void HandleCollisions(Captain& cap, float dt, const std::vector<GameObject*>& coObjects) = 0;
	virtual ~CaptainState() = default;
};

