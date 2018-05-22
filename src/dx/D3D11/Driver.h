#pragma once
#include "../Shared.h"
#include <memory>
#include "../../Driver/Driver.h"
#include "Render.h"

namespace DX::D3D11 {
	class Driver {
		
	public:
		Driver() = delete;
		Driver(ComPtr<IDXGISwapChain> swap, 
			ComPtr<ID3D11Device> device,
			ComPtr<ID3D11DeviceContext> context,
			const VideoDriver::DisplayCreation creation,
			const VideoDriver::DisplayMode mode);

		static std::unique_ptr<Driver> CreateDevice(HWND hwnd, const VideoDriver::DisplayCreation creation, const VideoDriver::DisplayMode mode);
		void setupDefaults();
		void resize(unsigned int width, unsigned int height);
		void present();

	private:
		void setupRenderTargetView(); // Call First
		void setupDepthBuffer(); // Call 2nd
		void setupDepthStencil();
		void setupRasterDescription(); //Call 3rd
		
		VideoDriver::DisplayMode		mMode;
		VideoDriver::DisplayCreation	mCreation;

		ComPtr<IDXGISwapChain>			mSwapChain			= nullptr;
		ComPtr<ID3D11Device>			mDevice				= nullptr;
		ComPtr<ID3D11RenderTargetView>	mRenderTargetView	= nullptr;
		ComPtr<ID3D11Texture2D>			mDepthStencilBuffer	= nullptr;
		ComPtr<ID3D11DepthStencilState>	mDepthStencilState	= nullptr;
		ComPtr<ID3D11DepthStencilView>	mDepthStencilView	= nullptr;
		ComPtr<ID3D11RasterizerState>	mRasterState		= nullptr;
		Render							mRender;
	};
	 

}