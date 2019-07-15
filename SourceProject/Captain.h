#pragma once
#include "VisibleObject.h"

class Captain : public VisibleObject
{
private:
	void ProcessInput();
	void HandleNoCollisions(float dt);
	void HandleCollisions(float dt, const std::vector<GameObject*>& coObjects);

public:
	Captain(const Vector2 & spawnPos);

	void OnKeyDown(BYTE keyCode);
	void SetState(State state) override;
	void Update(float dt, const std::vector<GameObject*>& coObjects) override;
};

