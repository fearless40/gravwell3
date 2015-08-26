#pragma once


////////////////////////////////////////////////////////////////////
/* 
	Basic Usuage

	Inherit your application class from AppGame like so:
		class MyGameClass : public AppGame<MyGameClass> 

	MyGameClass must contain a function called Frame:
	bool MyGameClass::Frame() 

	You must provide an implementation for the function initalize;
	bool MyGameClass::initalize() 
	In this function you should register and create any windows and such code that you need
	to survive. If it fails the program will exit

*/
////////////////////////////////////////////////////////////////////

template <typename base>
class AppGame
{
private:
	// Singleton 
	static base * _me;

protected:
		HINSTANCE	hInst;
		HACCEL		hAccelTable;
		int			appResourceID;

public:
	// Constructor
	AppGame(HINSTANCE hinst, int resourceID) : hInst(hinst), appResourceID(resourceID)
	{ 
		_me = (base*)(this);
	}

	// Descrtuctor
	virtual ~AppGame() {  } 
	

	// Must override
	// Override in the child calls to do custom code
	// Remeber to register any window class and such stuff
	virtual bool initalize() =  0;

	int run() { 
		hAccelTable = LoadAccelerators(hInst, (LPCTSTR)appResourceID);;
		
		if( !initalize() )
			return -1;
		
		MSG msg;
		bool running = true;

		while( running )
		{
			if( PeekMessage(&msg, NULL, 0,0, PM_REMOVE ) )
			{
				if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg)) 
				{
					TranslateMessage(&msg);
					DispatchMessage(&msg);
				}

				if( msg.message == WM_QUIT ) 
				{
					running = false;
					break;
				}
			}

			running = _me->Frame();

		}
		return true;	
		
	}

	HINSTANCE	getHINSTANCE()		{ return hInst; }
	int			getAppResourceID()	{ return appResourceID;} 


	static base & get() { return *_me; }

};

template <typename base>
base * AppGame<base>::_me = nullptr;