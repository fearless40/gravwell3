#pragma once
//#include "../Shared.h"
//#include <memory>
#include "../Generic/Driver.h"
//#include "../Generic/ConstantBuffer.h"
//#include "Context.h"
//#include "GenericToD3D.h"
//#include "Buffer.h"
//#include "ShaderCompiler.h"



namespace Graphics::D3D11 {

	using ConstantBuffer = comptr<ID3D11Buffer>;
	using IndexBuffer = comptr<ID3D11Buffer>;
	using VertexBuffer = comptr<ID3D11Buffer>;
	using VertexShader = comptr<ID3D11VertexShader>;
	using PixelShader = comptr<ID3D11PixelShader>;
	

	class Driver {

	public:
		Driver() = delete;
		Driver(comptr<IDXGISwapChain> swap,
			comptr<ID3D11Device> device,
			comptr<ID3D11DeviceContext> context,
			const Graphics::Generic::DisplayMode mode);

		static std::unique_ptr<Driver> CreateDevice(HWND hwnd,
			const Graphics::Generic::DisplayMode mode);

		void setupDefaults();
		void resize(unsigned int width, unsigned int height);
		void present();

		float getWidth() { return mMode.width; }
		float getHeight() { return mMode.height; }


		ID3D11Device * get() const noexcept { return mDevice.get(); }

		ID3D11RenderTargetView * getRenderTargetView() const noexcept {
			return mRenderTargetView.get();
		}

		ID3D11DepthStencilView * getDepthStencil() const noexcept {
			return mDepthStencilView.get();
		}

		ID3D11DeviceContext * getContext() const noexcept {
			return mRender.get();
		}

		comptr<ID3D11DeviceContext> getContext_comptr() const noexcept {
			return mRender;
		}



	private:

		void setDefaultViewport();
		void setupRenderTargetView(); // Call First
		void setupDepthBuffer(); // Call 2nd
		void setupDepthStencil();
		void setupRasterDescription(); //Call 3rd

		

		Graphics::Generic::DisplayMode		mMode;

		comptr<IDXGISwapChain>			mSwapChain{ nullptr };
		comptr<ID3D11Device>			mDevice{ nullptr };
		comptr<ID3D11RenderTargetView>	mRenderTargetView{ nullptr };
		comptr<ID3D11Texture2D>			mDepthStencilBuffer{ nullptr };
		comptr<ID3D11DepthStencilState>	mDepthStencilState{ nullptr };
		comptr<ID3D11DepthStencilView>	mDepthStencilView{ nullptr };
		comptr<ID3D11RasterizerState>	mRasterState{ nullptr };
		comptr<ID3D11DeviceContext>		mRender{ nullptr };
		

		//friend class Graphics::D3D11::Render;
	};
	 

}