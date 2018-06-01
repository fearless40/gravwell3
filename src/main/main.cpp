// Gravwell 3.cpp : Defines the entry point for the application.
//

#include "../stdafx.h"
#include "resource.h"
//#include "D3D11App.h"
#include "App.h"


int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPTSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	auto app = App::CreateApp(hInstance, lpCmdLine);
	app->run();
	//D3D11App app(hInstance);

	return app.run();

}



