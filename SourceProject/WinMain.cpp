#include "pch.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int)
{
	Game::Instance().InitGame();
	Game::Instance().Run();
}