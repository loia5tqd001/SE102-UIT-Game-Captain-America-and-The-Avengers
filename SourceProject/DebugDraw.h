#pragma once

class DebugDraw
{
private:
	static bool isInDebugMode;
	static bool isDrawCrt;
	static std::unique_ptr<Font> fontDraw;

public:
	static void InitDefaultFont();
	static void ToggleDebugMode();
	static void ToggleCrtMode();
	static bool IsInDebugMode();
	static bool IsInDeepDebug();

	static void DrawSolidRect(const RectF& bbox, D3DCOLOR color);
	static void DrawRectOutLine(const RectF& bbox, D3DCOLOR color);
	static void DrawString(const std::string& str, const Vector2& pos, D3DCOLOR color);
	static void DrawString(const std::string& str, const Vector2& pos, D3DCOLOR color, UINT size, LPCSTR font);
	static void DrawCrt();
};
