#include "../stdafx.h"
#include "Resource.h"
#include "App.h"
#include "D3D11App.h"
#include "Window.h"

WindowsGame::WindowsGame(HINSTANCE hInstance, Util::CommandLineParameters clp) : mInst( hInstance ) {
	Window::RegisterWindowClasses(mInst, IDI_GRAVWELL3, IDI_SMALL, L"D3D Window", L"D3DWindow");
	mAccelTable = LoadAccelerators(mInst, (LPCTSTR)IDI_GRAVWELL3);
	setHWND(Window::createHWND(0, L"D3DWindow"));
	show();
}

void WindowsGame::run() {
	MSG msg;
	bool running = true;
	
	timer.start();

	while (running)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}

			if (msg.message == WM_QUIT)
			{
				running = false;
				break;
			}
		}

		next_frame();

	}
	return;
}

void WindowsGame::shutdown() {

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


struct D3D11App::InternalData {
	RefCountPointer<D3DWindow> main;
	TestClass * test;
};


D3D11App::D3D11App(HINSTANCE hinst) : AppGame(hinst, IDI_GRAVWELL3), mImpl(new D3D11App::InternalData)
{
	
}


D3D11App::~D3D11App(void)
{
}



bool D3D11App::initalize() 
{

	D3DWindow::RegisterWindowClass(hInst, IDI_GRAVWELL3, IDI_SMALL);
	mImpl->main = new D3DWindow();
	mImpl->main->initalize(false,false);
	mImpl->main->show();
		
	D3D11 & d3d = mImpl->main->getD3D();

	mImpl->test = new TestClass( d3d );

	auto rect = mImpl->main->getClientPos();

	if( mImpl->test->initalize( rect.width(), rect.height() ) )
		return true;

	return false;
}


bool D3D11App::Frame() 
{
	
	return mImpl->test->frame();
	
}

