#pragma once
#include "window.h"

class D3D11;

class D3DWindow : public Window
{
protected:
	D3D11 * d3d;

public:
	D3DWindow(void);
	~D3DWindow(void);

	bool initalize(bool VSync, bool FullScreen);

	static void RegisterWindowClass(HINSTANCE hInst, int AppID, int IconID);

	D3D11 & getD3D() { return *d3d; }

protected:
	void onSize( stdBounds newSize );


};

