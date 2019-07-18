#pragma once
#include "AbstractScene.h"

//Warning: Include header file
#include"Shield.h"

class CharlestonScene : public AbstractScene
{
private:
	std::unique_ptr<Map> map; 
	std::unique_ptr<Grid> grid; 	
	std::unique_ptr<Captain> cap; //Just for testing... 
	std::unique_ptr<Shield> shield;

public:
	CharlestonScene();
	void LoadResources () override;
	void Update(float dt) override;
	void Draw          () override;

	void OnKeyDown(BYTE keyCode) override;
	SoundId GetBgMusic() const override { return SoundId::CaptainTheme; }
};

