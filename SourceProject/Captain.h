#pragma once
#include "VisibleObject.h"

class Captain : public VisibleObject
{
private:
	static constexpr float WALKING_SPEED = 50.0f;

	void ProcessInput();
	void HandleNoCollisions(float dt);
	void HandleCollisions(float dt, const std::vector<GameObject*>& coObjects);

public:
	Captain(const Vector2 & spawnPos);
	Vector2 GetPos() { return pos; };
	int GetNx() { return nx; };
	void OnKeyDown(BYTE keyCode);
	void SetState(State state) override;
	void Update(float dt, const std::vector<GameObject*>& coObjects) override;
	static Captain& Instance();
};

