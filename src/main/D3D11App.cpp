#include "../stdafx.h"
#include "Resource.h"
#include "App.h"
#include "D3D11.h"
#include "D3D11App.h"
#include "D3DWindow.h"
#include "../d3d11/VertexTypeManager.h"

#define MICROTEST

#ifdef MICROTEST
	#include "../tests/MicroMangerTest.h"
	typedef Test::MicroManagerTest TestClass;
#else
	#include "../tests/LowLevelTest.h"
	typedef Test::LowLevelTest TestClass;
#endif





struct D3D11App::InternalData {
	RefCountPointer<D3DWindow> main;
	TestClass * test;
};


D3D11App::D3D11App(HINSTANCE hinst) : AppGame(hinst, IDI_GRAVWELL3), pimpl(new D3D11App::InternalData)
{
	
}


D3D11App::~D3D11App(void)
{
	if( pimpl && pimpl->test )
		delete pimpl->test;
	
	delete pimpl;
}



bool D3D11App::initalize() 
{
	D3DWindow::RegisterWindowClass(hInst, IDI_GRAVWELL3, IDI_SMALL);
	pimpl->main = new D3DWindow();
	pimpl->main->initalize(false,false);
	pimpl->main->show();
		
	D3D11 & d3d = pimpl->main->getD3D();

	VertexTypeManager::initalize(  &d3d );

	pimpl->test = new TestClass( d3d );

	auto rect = pimpl->main->getClientPos();

	if( pimpl->test->initalize( rect.width(), rect.height() ) )
		return true;

	return false;
}


bool D3D11App::Frame() 
{
	
	return pimpl->test->frame();
	
}

