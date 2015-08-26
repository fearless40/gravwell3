#pragma once

#include <list>
#include <d3d11.h>


class D3D11
{
protected:
	bool mVSyncEnabled;
	int mVideoCardMemory;
	char mVideoCardDescription[128];
	IDXGISwapChain			* mSwapChain;
	ID3D11Device			* mDevice;
	ID3D11DeviceContext		* mDeviceContext;
	ID3D11RenderTargetView	* mRenderTargetView;
	ID3D11Texture2D			* mDepthStencilBuffer;
	ID3D11DepthStencilState	* mDepthStencilState;
	ID3D11DepthStencilView	* mDepthStencilView;
	ID3D11RasterizerState	* mRasterState;

public:
	D3D11(void);
	~D3D11(void);

	struct ScreenMode {
		unsigned int width, height;
		unsigned int numerator, denominator;
	};

	// Initalization and clean up
	bool initalizeDisplay( HWND wnd, bool vSyncEnabled, bool fullScreen, const D3D11::ScreenMode & mode );
	bool getScreenSettings(  D3D11::ScreenMode & mode, int screenWidth, int screenHeight );
	bool resizeWindow( int width, int height );
	void shutdown();

	// Rendering
	void beginRender();
	void endRender();

	// Information
	ID3D11Device		* getDevice() { return mDevice; }
	ID3D11DepthStencilView * getDepthStencilView() { return mDepthStencilView; }
	ID3D11DeviceContext * getDeviceContext() { return mDeviceContext; }
	void				getVideoCardInfo( std::string & cardname, int & memSize );
	

	// bool getScreenModes(std::list<ScreenMode> & modes);

};

