#include "pch.h"

void TextBlock::Draw(D3DCOLOR strColor, D3DCOLOR keyCodeColor) const
{
	KeyCodeFonts::DrawString(strDraw, posDraw, strColor);
	if (keyCode != NULL)
	{
		const Vector2 posDrawKeyCode = posDraw + Vector2{ 136.0f, 0.0f };
		KeyCodeFonts::DrawKeyCode(keyCode, posDrawKeyCode, keyCodeColor);
	}
}

Settings::Settings()
{
	// Init string draw for textblocks
	textblocks[TextBlocks::Left   ].strDraw = "LEFT";
	textblocks[TextBlocks::Right  ].strDraw = "RIGHT";
	textblocks[TextBlocks::Up     ].strDraw = "UP";
	textblocks[TextBlocks::Down   ].strDraw = "DOWN";
	textblocks[TextBlocks::Jump   ].strDraw = "JUMP";
	textblocks[TextBlocks::Attack ].strDraw = "ATTACK";
	textblocks[TextBlocks::Default].strDraw = "DEFAULT";

	// Init position draw for textblocks
	for (int i = TextBlocks::Left; i < TextBlocks::Count; i++) {

		if (i < TextBlocks::CountKCtrl) {
			textblocks[i].posDraw.x = 52.0f;
			textblocks[i].posDraw.y = 42.0f + i * 25.0f;
		}
		else { // TextBlocks::Default         
			textblocks[i].posDraw.x = 92.0f;
			textblocks[i].posDraw.y = 45.0f + i * 25.0f;
		}
	}

	// Init keyCode for textblocks
	ResetDefaultKeyControl();
}

void Settings::ResetDefaultKeyControl()
{
	textblocks[ TextBlocks::Left   ].keyCode = VK_LEFT;
	textblocks[ TextBlocks::Right  ].keyCode = VK_RIGHT;
	textblocks[ TextBlocks::Up     ].keyCode = VK_UP;
	textblocks[ TextBlocks::Down   ].keyCode = VK_DOWN;
	textblocks[ TextBlocks::Attack ].keyCode = 'Z';
	textblocks[ TextBlocks::Jump   ].keyCode = 'X';		
}

BYTE Settings::Get(KeyControls kControl) const
{
	assert(kControl >= KeyControls::Left && kControl < KeyControls::CountKCtrl);
	return textblocks[kControl].keyCode;
}

bool Settings::IsKeyControl(BYTE keyCode) const
{
	for (int i = 0; i < (int)KeyControls::CountKCtrl; i++)
		if (textblocks[(KeyControls)i].keyCode == keyCode)
			return true;
	return false;
}

KeyControls Settings::GetKControl(BYTE keyCode) const
{
	assert(IsKeyControl(keyCode));
	for (int i = 0; i < (int)KeyControls::CountKCtrl; i++)
		if (textblocks[(KeyControls)i].keyCode == keyCode)
			return KeyControls(i);
	return KeyControls::Count;
}

void Settings::SetOpening(bool isOpening)
{
	isSceneOpening = isOpening;
	isEditing = false;
}

bool Settings::IsValidKeyControl(BYTE keyCode)
{
	if (keyCode >= VK_NUMPAD0 && keyCode <= VK_NUMPAD9)     return true;
	if (keyCode >= '0' && keyCode <= '9')                   return true;
	if (keyCode >= 'A' && keyCode <= 'Z' && keyCode != 'M') return true;
	if (keyCode >= VK_LEFT && keyCode <= VK_DOWN)           return true;
	if (keyCode == VK_SPACE)                                return true;
	return false;
}

void Settings::Draw() const
{
	if (!isSceneOpening) return;

	// semi-transparent-black background
	Game::Instance().FillColor(Colors::DarkTransparentBlack);
	
	// title 'Settings'
	KeyCodeFonts::DrawString("Settings", { 90.0f, 10.0f }, Colors::White);

	// Draw frame background
	Game::Instance().DrawOutLine( {  17.0f,  29.0f, 239.0f, 215.0f }, 3, Colors::DimBlue); // center
	Game::Instance().DrawOutLine( {  10.0f,  22.0f,  20.0f,  32.0f }, 3, Colors::DimBlue); // topleft
	Game::Instance().DrawOutLine( { 236.0f,  22.0f, 246.0f,  32.0f }, 3, Colors::DimBlue); // topright
	Game::Instance().DrawOutLine( {  10.0f, 212.0f,  20.0f, 222.0f }, 3, Colors::DimBlue); // bottomleft
	Game::Instance().DrawOutLine( { 236.0f, 212.0f, 246.0f, 222.0f }, 3, Colors::DimBlue); // bottomright

	// Draw textblocks
	for (int i = TextBlocks::Left; i < TextBlocks::Count; i++)
	{
		if (i != curCursor) { // normal textblock
			textblocks[i].Draw(Colors::White, Colors::White);
		} 
		else if (isEditing) { // selected textblock editing
			textblocks[i].Draw(Colors::DimRed , Colors::DimRed);
		} 
		else { // selected textblock but not editing
			textblocks[i].Draw(Colors::DimBlue, Colors::DimBlue);
		}
	}
}

void Settings::OnKeyDown(BYTE keyCode)
{
	if (isEditing) // selected textblock is in editing
	switch (curCursor) {
		case TextBlocks::Left:
		case TextBlocks::Right:
		case TextBlocks::Up:
		case TextBlocks::Down:
		case TextBlocks::Jump:
		case TextBlocks::Attack:
			if (keyCode == VK_RETURN) {
				isEditing = false;
				Sounds::PlayAt(SoundId::Cursor);
			} 
			else if (IsValidKeyControl(keyCode)){
				TextBlock& curTextBlock = textblocks[curCursor];

				for (int i = 0; i < KeyControls::CountKCtrl; i++) { // change keyControl if an exist textblock has same key
					if (textblocks[i].keyCode == keyCode) {
						textblocks[i].keyCode = curTextBlock.keyCode;
						break;
					}
				}
				curTextBlock.keyCode = keyCode;
				Sounds::PlayAt(SoundId::Cursor);
			}
			break;

		default:
			ThrowMyException("curCursor has an invalid value!!");
	}

	else // isn't editing any textblock
	switch (keyCode) {
		case VK_UP: 
			if (curCursor == TextBlocks::Left) curCursor = TextBlocks::Default;
			else                               curCursor = TextBlocks( curCursor - 1 );
			Sounds::PlayAt(SoundId::Cursor);
			break;		

		case VK_DOWN: 
			if (curCursor == TextBlocks::Default) curCursor = TextBlocks::Left;
			else                                  curCursor = TextBlocks( curCursor + 1 );
			Sounds::PlayAt(SoundId::Cursor);
			break;

		case VK_RETURN: 
			if (curCursor == TextBlocks::Default) ResetDefaultKeyControl(); 
			else                                  isEditing = true        ; 
			Sounds::PlayAt(SoundId::Cursor);
			break;
	}
}

Settings& Settings::Instance()
{
	static Settings instance;
	return instance;
}
