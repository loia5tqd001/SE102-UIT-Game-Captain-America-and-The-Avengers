#pragma once
#include "AbstractScene.h"

//Warning: Include header file
#include"Captain.h"

class CharlestonScene : public AbstractScene
{
private:
	std::unique_ptr<Map>   map; // responsible for drawing map background
	
	std::unique_ptr<Captain> cap; //Just for testing... 
public:
	CharlestonScene();
	void LoadResources () override;
	void Update(float dt) override;
	void Draw          () override;

	void OnKeyDown(BYTE keyCode) override;
	SoundId GetBgMusic() const override { return SoundId::CaptainTheme; }
};

