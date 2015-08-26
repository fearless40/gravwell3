#include "../stdafx.h"

#include <dxgi.h>
#include <d3dcommon.h>
#include <d3d11.h>
#include <DirectXMath.h>

#include "D3DWindow.h"
#include "../d3d11/D3D11.h"


D3DWindow::D3DWindow(void)
{
	d3d = new D3D11();
}


D3DWindow::~D3DWindow(void)
{
	delete d3d;
}

bool D3DWindow::initalize(bool VSync, bool FullScreen) 
{
	setHWND( Window::createHWND( 0, L"D3DWindow" ) );
	D3D11::ScreenMode sm;
	
	// Get the screen mode settings
	d3d->getScreenSettings(sm ,1024,768);

	return d3d->initalizeDisplay( getHWND(), VSync, FullScreen, sm );
}


void D3DWindow::RegisterWindowClass(HINSTANCE hInst, int AppID, int IconID)
{
	Window::RegisterWindowClasses(hInst, AppID, IconID, L"D3D Window", L"D3DWindow");
}

void D3DWindow::onSize( stdBounds newSize )
{
	d3d->resizeWindow( newSize.width(), newSize.height() );
}
	

