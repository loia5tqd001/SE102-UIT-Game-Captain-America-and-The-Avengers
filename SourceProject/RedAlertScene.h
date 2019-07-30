#pragma once
#include "AbstractScene.h"

class RedAlertScene : public AbstractScene
{
private:
	std::unique_ptr<Map> map; 
	std::unique_ptr<Grid> grid;
	std::unique_ptr<Captain> cap;

	bool bossIsDestroyed = false;
	//std::unique_ptr<DynamiteNapalm> miniboss;
public:
	RedAlertScene();
	void LoadResources () override;
	void Update(float dt) override;
	void Draw          () override;

	void OnKeyDown(BYTE keyCode) override;
	void OnKeyUp(BYTE keyCode) override;
	SoundId GetBgMusic() const override { return SoundId::RedAlert; }
};

