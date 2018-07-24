#include "../stdafx.h"
#include "Resource.h"
#include "../util/CmdLineParser.h"
#include "Window.h"
#include "WindowsGame.h"
#include "../engine/GameEvent.h"
#include "../engine/Event.h"



WindowsGame::WindowsGame(HINSTANCE hInstance, Util::CommandLineParameters clp) : mInst( hInstance ) {
	Window::RegisterWindowClasses(mInst, IDI_GRAVWELL3, IDI_SMALL, L"D3D Window", L"D3DWindow");
	mAccelTable = LoadAccelerators(mInst, (LPCTSTR)IDI_GRAVWELL3);
	setHWND(Window::createHWND(0, L"D3DWindow"));
	show();
}

void WindowsGame::run() {
	MSG msg;
	mRunning = true;
	
	timer.start();

	while (mRunning)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (!TranslateAccelerator(msg.hwnd, mAccelTable, &msg))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}

			if (msg.message == WM_QUIT)
			{
				mRunning = false;
				break;
			}
		}

		next_frame();

	}
	return;
}

void WindowsGame::shutdown() {
	mRunning = false;
}

void WindowsGame::next_frame() 
{
	timer.startFrame();
	if (timer.isUpdateRequired()) {
		next_game_step(timer.getInterpolation());
		timer.performUpdate();
	}
	else {
		next_render_step(timer.getInterpolation());
	}

}

void WindowsGame::next_game_step(float step) {
	Events::Event<Engine::NextLogicFrame>::Fire({ step });
}

void WindowsGame::next_render_step(float step) {
	Events::Event<Engine::NextRenderFrame>::Fire({ step });
}

void WindowsGame::onSize(stdBounds newSize) {

}
