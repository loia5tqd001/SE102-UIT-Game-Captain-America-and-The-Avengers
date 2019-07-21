#pragma once

using TextBlocks = enum KeyControls
{
	Left, Right, Up, Down, Jump, Attack, // textblock is also keycontrol
	Default,                             // textblock is not keycontrol
	Count, CountKCtrl = Count - 1
};

struct TextBlock
{
	std::string strDraw;
	Vector2     posDraw;
	BYTE        keyCode = NULL; // textblock 'Default' and 'Back' don't have corresponding keyCode 
	void Draw(D3DCOLOR strColor, D3DCOLOR keyCodeColor) const;
};

class Settings
{
private:
	bool isSceneOpening = false;
	bool isEditing = false;
	TextBlock  textblocks [TextBlocks::Count];
	TextBlocks curCursor = TextBlocks::Left;

private:
	static bool IsValidKeyControl(BYTE keyCode);
	void ResetDefaultKeyControl();

public:
	void SetOpening(bool isOpening);
	inline bool IsOpening() const { return isSceneOpening; }
	inline bool IsEditing() const { return isEditing; }
	BYTE Get(KeyControls kControl) const;
	bool IsKeyControl(BYTE keyCode) const;
	KeyControls GetKControl(BYTE keyCode) const;
	void Draw() const;
	void OnKeyDown(BYTE keyCode);

private:
	Settings();
	Settings(const Settings&) = delete;

public:
	static Settings& Instance();
};
