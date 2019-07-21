#pragma once

class CharlestonScene : public AbstractScene
{
private:
	std::unique_ptr<Map> map; 
	std::unique_ptr<Grid> grid; 	
	std::unique_ptr<Captain> cap; //Just for testing... 

public:
	CharlestonScene();
	void LoadResources () override;
	void Update(float dt) override;
	void Draw          () override;

	void OnKeyDown(BYTE keyCode) override;
	void OnKeyUp(BYTE keyCode) override;

	SoundId GetBgMusic() const override { return SoundId::CaptainTheme; }
};

