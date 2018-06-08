#pragma once
#include "../Shared.h"
#include <memory>
#include "../Generic/Driver.h"
#include "Context.h"
#include "GenericToD3D.h"
#include "Buffer.h"
#include "ShaderCompiler.h"


namespace Graphics::D3D11 {
	class Driver {
		
	public:
		Driver() = delete;
		Driver(ComPtr<IDXGISwapChain> swap, 
			ComPtr<ID3D11Device> device,
			ComPtr<ID3D11DeviceContext> context,
			const Graphics::Generic::DisplayMode mode);

		static std::unique_ptr<Driver> CreateDevice(HWND hwnd, 
			const Graphics::Generic::DisplayMode mode);
		
		void setupDefaults();
		void resize(unsigned int width, unsigned int height);
		void present();

		// Creation Functions
		template <typename T>
		Buffer createConstantBuffer(const Graphics::Generic::Buffer<T> & buf) {
			return createBuffer(buf.getMemory(), buf.getSize(),
				D3D11_USAGE_DYNAMIC,
				D3D11_BIND_CONSTANT_BUFFER, 
				D3D11_CPU_ACCESS_WRITE);
		}

		template <typename T>
		Buffer createVertexBuffer(const Graphics::Generic::Buffer<T> & buf, Graphics::Generic::BufferBinding binding) {
			return createBuffer(buf.getMemory(), buf.getSize(),
				D3D11_BIND_VERTEX_BUFFER,
				Graphics::D3D::Conversion::BufferBinding(binding),
				Graphics::D3D::Conversion::BufferBindingToCPUAccess(binding));
		}

		template <typename T>
		Buffer createIndexBuffer(const Graphics::Generic::Buffer<T> & buf, Graphics::Generic::BufferBinding binding) {
		return createBuffer(buf.getMemory(), buf.getSize(),
			D3D11_BIND_INDEX_BUFFER,
			Graphics::D3D::Conversion::BufferBinding(binding),
			Graphics::D3D::Conversion::BufferBindingToCPUAccess(binding));
		}

		ID3D11Device * get() const noexcept { return mDevice.Get(); }

		template< typename VertexDescription >
		void registerVertexDescription(VertexDescription & vd) {
			if constexpr (VertexDescription::requires_compilation::value) {
				auto shader = ShaderCompiler::compile(vd.layoutstring());
				auto inputarr = vd.layoutarray();
				mDevice->CreateInputLayout(inputarr.data(), inputarr.size(), shader->GetBufferPointer(), shader->GetBufferSize(), &vd);
			}
			else {
				auto shader = vd.compiledshader();
				auto inputarr = vd.layoutarray();
				mDevice->CreateInputLayout(inputarr.data(), inputarr.size(), shader.data(), shader.size(), &vd);
			}
			
		}

	private:
		void setupRenderTargetView(); // Call First
		void setupDepthBuffer(); // Call 2nd
		void setupDepthStencil();
		void setupRasterDescription(); //Call 3rd

		ComPtr<ID3D11Buffer> createBuffer(void * mem, unsigned int memSize,
			D3D11_USAGE bufferMemoryType,
			unsigned int bindFlags, 
			unsigned int CPUAccessFlags);
		
		Graphics::Generic::DisplayMode		mMode;
		
		ComPtr<IDXGISwapChain>			mSwapChain			= nullptr;
		ComPtr<ID3D11Device>			mDevice				= nullptr;
		ComPtr<ID3D11RenderTargetView>	mRenderTargetView	= nullptr;
		ComPtr<ID3D11Texture2D>			mDepthStencilBuffer	= nullptr;
		ComPtr<ID3D11DepthStencilState>	mDepthStencilState	= nullptr;
		ComPtr<ID3D11DepthStencilView>	mDepthStencilView	= nullptr;
		ComPtr<ID3D11RasterizerState>	mRasterState		= nullptr;
		Context							mRender;
	};
	 

}