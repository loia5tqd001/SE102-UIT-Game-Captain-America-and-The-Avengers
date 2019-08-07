#pragma once
#include "AbstractScene.h"

enum class PittsburghScenePart { MainMap, Room1, Room2 };

class PittsburghScene : public AbstractScene
{
private:
	std::unique_ptr<Captain> cap;
	std::unique_ptr<Grid> grid;
	std::unique_ptr<Map> mapDark, mapLight; 
	PittsburghScenePart currentScenePart = PittsburghScenePart::MainMap;

public:
	PittsburghScene();
	void LoadResources () override;
	void Update(float dt) override;
	void Draw          () override;
	void OnKeyDown(BYTE keyCode) override;
	void OnKeyUp(BYTE keyCode) override;
	SoundId GetBgMusic() const override { return SoundId::CaptainTheme; }
	void SetCapPos(Vector2 pos);
	void Teleport() override;
	void ClampCaptainAndCamera();

	bool isPauseGameExceptDoor = false;
};

