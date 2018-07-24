// Gravwell 3.cpp : Defines the entry point for the application.
//

#include "../stdafx.h"
#include <string_view>
#include "resource.h"
#include "../engine/Engine.h"
#include "WindowsGame.h"




int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPTSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	auto parsedcommandline = Engine::Initalize(std::wstring_view { lpCmdLine });
	WindowsGame game{ hInstance, parsedcommandline };
	game.run();
	Engine::Shutdown();
}



