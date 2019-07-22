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
	virtual void OnKeyUp(Captain& cap, BYTE keyCode) = 0;
	virtual void OnKeyDown(Captain& cap, BYTE keyCode) = 0;
	virtual void Update(Captain& cap, float dt, const std::vector<GameObject*>& coObjects) = 0; // captain delegates his update call to this function
	virtual void HandleCollisions(Captain& cap, float dt, const std::vector<GameObject*>& coObjects) = 0;
	virtual ~CaptainState() = default;

protected:
	// define properties for subclasses here. Those states need to communicate via Data, which need unified property name
	static constexpr auto CAN_JUMP_HIGHER = "can-higher";
	static constexpr auto IS_JUMP_RELEASED = "is-jump-released";
	static constexpr auto IS_TO_SIT_TACKLE = "is-to-sit-tackle";


	static constexpr float WATER_FLOW_SPEED = -20.0f;
	static constexpr float CAP_SWIMMING_SPEED = 50.0f;


	static constexpr auto JUMP_SPEED_VER = 170.0f;
	static constexpr auto JUMP_SPEED_HOR = 45.0f;
};

