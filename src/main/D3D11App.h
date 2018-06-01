#pragma once
#include "App.h"
#include "Window.h"
#include "TimeStep.h"


/*class D3D11App : public AppGame<D3D11App>
{
private:
	struct InternalData;
	
	std::unique_ptr<InternalData> mImpl;


public:
	D3D11App(HINSTANCE hinst);
	~D3D11App(void);

	bool initalize();

	bool Frame();
};*/

class WindowsGame : public App<WindowsGame>, Window {
public:
	WindowsGame(HINSTANCE hInstance, Util::CommandLineParameters clp);

	void run();
	void shutdown();

private:

	void next_frame(); 

	HINSTANCE mInst;
	HACCEL	  mAccelTable;
	Util::TimeStep timer;
};