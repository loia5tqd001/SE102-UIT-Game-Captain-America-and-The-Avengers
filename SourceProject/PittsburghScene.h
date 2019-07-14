#pragma once
#include "AbstractScene.h"

class PittsburghScene : public AbstractScene
{
private:
	std::unique_ptr<Map> mapDark, mapLight; // responsible for drawing map background
	Map* curMap = nullptr;

public:
	PittsburghScene();
	void LoadResources () override;
	void Update(float dt) override;
	void Draw          () override;

	void OnKeyDown(BYTE keyCode) override;
	SoundId GetBgMusic() const override { return SoundId::CaptainTheme; }
};

