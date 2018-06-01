#pragma once
#include <spdlog/spdlog.h>
#include "../util/CmdLineParser.h"


template <typename AppType>
class App {
private:
	static std::unique_ptr<App> theApp;
	static shared_ptr<spdlog::logger> mDefaultLog;

	static AppType *  base() { return static_cast<AppType>(theApp.get()); }
	

public:

	enum class ShutdownReasons {
		User,
		NetworkRequest,
		Error,
		SystemRequest
	};

	static void run() {
		base()->run();
	}

	static void shutdown(ShutdownReasons reason) {
		base()->shutdown(reason);
	}

	template< typename Arguments >
	static AppType * CreateApp(const Arguments & AppArgs, const char * cmdline) {
		mDefaultLog = spdlog::stdout_color_mt("default_log");
		auto cmdparameters = Util::ProcessCmdLine(cmdline);
		theApp = std::make_unique(std::forward(AppArgs), cmdparameters);
		return theApp.get();
	}

	static AppType * get() { return theApp.get(); }


	static auto default_log() { return mDefaultLog; }
};

template <typename AppType>
static shared_ptr<spdlog::logger> App<AppType>::mDefaultLog;

template <typename AppType>
std::unique_ptr<App> App<AppType>::theApp = nullptr;



/*
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

template <typename T, typename Arguments>
std::unique_ptr<T> CreateApp(const Arguments & AppArgs, const char * cmdline) {
App::ProcessCommandLine(cmdline);
}

};



template <typename base>
base * AppGame<base>::_me = nullptr;
*/