#pragma once
//#include "../Shared.h"
//#include <memory>
#include "../Generic/Driver.h"
#include "../Generic/ConstantBuffer.h"
//#include "Context.h"
//#include "GenericToD3D.h"
#include "Buffer.h"
//#include "ShaderCompiler.h"



namespace Graphics::D3D11 {
	class Driver;

	using ConstantBuffer = comptr<ID3D11Buffer>;
	//using IndexBuffer = Graphics::Generic::IndexBuffer<Graphics::D3D11::Buffer>;
	//using VertexBuffer = Graphics::Generic::VertexBuffer<Graphics::D3D11::Buffer>;
	
	
	
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

		// Creation Functions
		ConstantBuffer createConstantBuffer(void * memory, std::size_t size) {
			return createBuffer(memory, size,
				D3D11_USAGE_DYNAMIC,
				D3D11_BIND_CONSTANT_BUFFER,
				D3D11_CPU_ACCESS_WRITE);
		}




		/*
		template <typename T>
		Buffer createVertexBuffer(const Graphics::Generic::Buffer<T> & buf, Graphics::Generic::BufferBinding binding) {
			return createBuffer(buf.getMemory(), buf.getSize(),
				D3D11_BIND_VERTEX_BUFFER,
				Graphics::D3D::Conversion::BufferBinding(binding),
				Graphics::D3D::Conversion::BufferBindingToCPUAccess(binding));
		}

		template <typename T>
		DriverTraits::IndexBuffer createIndexBuffer(const Graphics::Generic::Buffer<T> & buf, Graphics::Generic::BufferBinding binding, Graphics::Generic::IndexBuffer::Format fmt) {
			return IndexBuffer{ createBuffer(buf.getMemory(), buf.getSize(),
				D3D11_BIND_INDEX_BUFFER,
				Graphics::D3D::Conversion::BufferBinding(binding),
				Graphics::D3D::Conversion::BufferBindingToCPUAccess(binding)),
				IndexFormat(fmt) };
		}
		*/
		ID3D11Device * get() const noexcept { return mDevice.get(); }

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

		void setDefaultViewport();
		void setupRenderTargetView(); // Call First
		void setupDepthBuffer(); // Call 2nd
		void setupDepthStencil();
		void setupRasterDescription(); //Call 3rd

		ConstantBuffer createBuffer(void * mem, unsigned int memSize,
			D3D11_USAGE bufferMemoryType,
			unsigned int bindFlags,
			unsigned int CPUAccessFlags);

		Graphics::Generic::DisplayMode		mMode;

		comptr<IDXGISwapChain>			mSwapChain{ nullptr };
		comptr<ID3D11Device>			mDevice{ nullptr };
		comptr<ID3D11RenderTargetView>	mRenderTargetView{ nullptr };
		comptr<ID3D11Texture2D>			mDepthStencilBuffer{ nullptr };
		comptr<ID3D11DepthStencilState>	mDepthStencilState{ nullptr };
		comptr<ID3D11DepthStencilView>	mDepthStencilView{ nullptr };
		comptr<ID3D11RasterizerState>	mRasterState{ nullptr };
		comptr<ID3D11DeviceContext>		mRender{ nullptr };
		
	};
	 

}