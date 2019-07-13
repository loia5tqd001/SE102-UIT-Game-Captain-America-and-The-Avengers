#pragma once
#include "AbstractScene.h"

class BossCharlestonScene : public AbstractScene
{
private:
	std::unique_ptr<Map>   map; // responsible for drawing map background

public:
	BossCharlestonScene();
	void LoadResources () override;
	void Update(float dt) override;
	void Draw          () override;

	void OnKeyDown(BYTE keyCode) override;
	SoundId GetBgMusic() const override { return SoundId::BossBattle; }
};

