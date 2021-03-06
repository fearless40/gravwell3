#include "../stdafx.h"
#include "Window.h"
#include <boost\bind.hpp>



const wchar_t propName[] = L"ClassPointer";

Window::Window(void) : hwnd(0)
{
}

Window::Window( HWND wnd ) : hwnd( wnd )
{
	SetClassPointer( this );
}

Window::~Window(void)
{
	_RemoveHWNDProperty();
	DestroyWindow(hwnd);
}

void Window::_RemoveHWNDProperty()
{
	if( hwnd == 0 )
		return;
	
	HANDLE mHandle = RemoveProp( this->hwnd, propName );
	if( mHandle != 0 )
	{
		// Delete the memory associated with the handle
		GlobalFree( mHandle );
	}
}

Window::RectI Window::getClientPos() const
{
	RECT r;
	GetClientRect( getHWND(), &r );
	return RectI( r.left, r.top, r.right, r.bottom );
}

void Window::setParent( WindowPtr parent )
{
	onParent( parent );
}

void Window::setHWND( HWND wnd )
{
	// Remove the old HWND property
	_RemoveHWNDProperty();

	// Copy the value
	hwnd = wnd;

	// Set the new window property
	SetClassPointer( this );
}

void Window::addChild( Window::WindowPtr child )
{
	child->onParent(  this  );
	children[ child->getHWND() ] = child ;
}

void Window::removeChild( Window::WindowPtr child )
{
	child->onParent( WindowPtr() );
	children.erase( child->getHWND() );
}

void Window::show()
{
	ShowWindow(hwnd, SW_SHOW);
}

void Window::onDestroy()
{
	// DO nothing in the default imp
}

void Window::onParent( Window::WindowPtr parent )
{
	if( parent )
	{
		// Register for the onSize event
		
	}
}

void Window::onSize( RectI newSz )
{
	
}

bool Window::onCommandDispatch( HWND wnd, int wmEvent )
{
	if( wnd == hwnd )
	{
		return onCommand(wmEvent);
	}
	else
	{
		// Dispatch the event to a child window

		ChildList::iterator it( children.find(wnd) );
		if( it != children.end() )
			return it->second->onCommandDispatch( wnd, wmEvent );
	}
	return false;
}

void Window::onAcclerator( int id )
{

}

bool Window::onCommand( int wmEvent )
{
	return true;
}

void Window::onCreateStatic()
{

}

void Window::onMenu( int id )
{

}

void Window::onPaint( HDC dc, RectI bounds )
{

}

int Window::WindowProc( UINT message, WPARAM wParam, LPARAM lParam )
{
	return DefWindowProc(hwnd, message, wParam, lParam);
}



void Window::SetClassPointer(Window * wPtr)
{
	// Check if the window is valid
	if( !wPtr->isValid() )
		return;

	typedef unsigned char byte;

	// Check to make sure the window is not already registered
	if (GetProp( wPtr->getHWND(), propName ) != 0 )
		wPtr->_RemoveHWNDProperty();


	// Generate a handle and get a pointer to the mem
	HANDLE mHandle = GlobalAlloc( GPTR, sizeof(void *) );
	byte * mPtr = (byte *) GlobalLock( mHandle );

	// Copy the this pointer
	memcpy(mPtr, &wPtr, sizeof(void *) );
	
	// Unlock the handle
	GlobalUnlock(mHandle);

	// Push it into the windows property list
	SetProp( wPtr->getHWND(), propName, mHandle );
}

Window * Window::GetClassPointer(HWND hwnd)
{

	Window * wPtr = 0;

	if(hwnd==0)
		return 0;

	HANDLE mHandle = GetProp( hwnd, propName );

	if(mHandle)
	{
		unsigned char * mPtr = (unsigned char *) GlobalLock( mHandle );

		// Copy the this pointer
		memcpy(&wPtr, mPtr, sizeof(void *) );
	
		// Unlock the handle
		GlobalUnlock(mHandle);
	}

	return wPtr;
}

HWND Window::createHWND(HWND parent, std::wstring WindowsClass, RectI bounds , DWORD style, DWORD ExtendedStyles, std::wstring title,  HINSTANCE hinst, HMENU hmenu )
{
		
	int width = bounds.width() <= 0 ? CW_USEDEFAULT : bounds.width();
	int height = bounds.height() <= 0 ? CW_USEDEFAULT : bounds.height();

	HWND hwnd = CreateWindowEx ( ExtendedStyles,
								WindowsClass.c_str(),
								title.c_str(),
								style,
								CW_USEDEFAULT,
								CW_USEDEFAULT,
								width,
								height,
								parent,
								hmenu,
								hinst,
								0 );
	
	return hwnd;
}

Window::WindowPtr Window::createWindow( std::wstring WindowsClass, RectI bounds , DWORD style, DWORD ExtendedStyles, std::wstring title, WindowPtr parent, HINSTANCE hinst, HMENU hmenu )
{
	HWND pHWND = 0;
	if( parent )
		pHWND = parent->getHWND();
	
	HWND hwnd = Window::createHWND(pHWND, WindowsClass, bounds, style, ExtendedStyles, title, hinst, hmenu ); 

	WindowPtr win =  new Window( hwnd );
	 

	 // Fire the virtual event to allow the class to make modifications to the code
	 win->onCreateStatic();
	 
	 // If a parent window was specified then make sure the class knows
	 if( parent ) 
		 parent->addChild( win );

	 return win;
}

Window::RectI Window::fromRECT( const RECT & r )
{
	return RectI( r.left, r.top, r.right, r.bottom );
}

std::wstring Window::RegisterWindowClasses(HINSTANCE hInst, int AppResourceID, int IconResourceID, std::wstring wTitle, std::wstring wClass)
{
	
	//LoadString(hInst, IDS_APP_TITLE, wTitle, 100);
	//LoadString(hInst, IDC_TERMINAL, wClass, 100);

	std::wstring windowClass(wClass);

	WNDCLASSEX wcex;

	wcex.cbSize			= sizeof(WNDCLASSEX); 
	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= (WNDPROC)Window::WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInst;
	wcex.hIcon			= LoadIcon(hInst, (LPCTSTR)AppResourceID);
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= NULL; //(HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= (LPCTSTR)AppResourceID;
	wcex.lpszClassName	= wClass.c_str();
	wcex.hIconSm		= LoadIcon(wcex.hInstance, (LPCTSTR)IconResourceID);

	ATOM value = RegisterClassEx(&wcex);

	
// Register a blank window type for use by child windows
	wcex.cbSize			= sizeof(WNDCLASSEX); 
	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= (WNDPROC)Window::WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInst;
	wcex.hIcon			= 0;
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= 0;
	wcex.lpszMenuName	= 0;
	wcex.lpszClassName	= L"BlankWindow";
	wcex.hIconSm		= 0;

	RegisterClassEx(&wcex);

	return windowClass;
}

int CALLBACK Window::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	Window * win = Window::GetClassPointer(hWnd);
	
	if(!win)
		return DefWindowProc(hWnd, message, wParam, lParam);

	int wmId    = LOWORD(wParam); 
	int wmEvent = HIWORD(wParam); 
	
	switch( message )
	{
	case WM_COMMAND:

		
		// Occurs for children controls
		if(wmEvent > 1 ) 
		{
			win->onCommandDispatch( (HWND) lParam, wmEvent );
		}
		else if(wmEvent == 1) // Handle Accelerators
		{
			win->onAcclerator( wmId );
		}
		else 	// Handle menu events
		{
			win->onMenu( wmId );
		}
		return win->WindowProc(message, wParam, lParam);
	case WM_SIZE:
        win->onSize(RectI(0,0, LOWORD(lParam), HIWORD(lParam)));	
		break;

	case WM_PAINT:
		{
			PAINTSTRUCT ps;
			BeginPaint( hWnd, &ps );
				win->onPaint( ps.hdc, RectI(ps.rcPaint.left, ps.rcPaint.top,
											ps.rcPaint.right, ps.rcPaint.bottom ) );
			EndPaint( hWnd, &ps );
		}
		break;
	
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	
	default:
		return win->WindowProc(message, wParam, lParam);
	}
	return 0;
}