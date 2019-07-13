#pragma once
#include "AbstractScene.h"

class CharlestonScene : public AbstractScene
{
private:
	std::unique_ptr<Map>   map; // responsible for drawing map background

public:
	CharlestonScene();
	void LoadResources () override;
	void Update(float dt) override;
	void Draw          () override;

	void OnKeyDown(BYTE keyCode) override;
	SoundId GetBgMusic() const override { return SoundId::GamePlay; }
};

