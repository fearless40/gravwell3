// Gravwell 3.cpp : Defines the entry point for the application.
//

#include "../stdafx.h"
#include "Gravwell 3.h"
#include "D3D11App.h"


int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPTSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	D3D11App app(hInstance);

	return app.run();

}



