#pragma once

class Window
{
private:
	static constexpr char* WINDOW_CLASS_NAME = "MainWindow" ;
	static constexpr char* WINDOW_TITLE      = "SE102 - UIT";
	static constexpr UINT  SCREEN_WIDTH      = 256;
	static constexpr UINT  SCREEN_HEIGHT     = 240;

	HINSTANCE hInstance = nullptr;
	HWND	  hWnd		= nullptr;
	std::bitset<256>    keyStates; // 256 is the maximum number of key codes possible

public:
	void InitWindow();
	bool ProcessMessage() const;
	void SetTitleToFps() const;
	bool IsKeyPressed(BYTE keyCode) const { return keyStates.test(keyCode); }

	static LRESULT CALLBACK WinProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	static void ShowMessageBox(LPCSTR message, LPCSTR title = "", UINT type = MB_OK);

	inline const UINT GetWidth () const { return SCREEN_WIDTH ;}
	inline const UINT GetHeight() const { return SCREEN_HEIGHT;}
	inline const HWND GetHWnd  () const { return hWnd         ;}	
	inline const Rect  GetWindowFrame() const { return { 0u  , 0u  , GetWidth(), GetHeight() }; }
	inline const RectF GetWindowBbox () const { return { 0.0f, 0.0f, GetWidth(), GetHeight() }; }

private:
	Window() = default;
	Window(const Window&) = delete;

public:
	static Window& Instance();
};

