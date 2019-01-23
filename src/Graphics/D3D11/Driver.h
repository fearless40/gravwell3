#pragma once
//#include "../Shared.h"
//#include <memory>
#include "../Generic/Driver.h"
#include "../Generic/ConstantBuffer.h"
//#include "Context.h"
//#include "GenericToD3D.h"
#include "Buffer.h"
#include "ShaderCompiler.h"
#include "BufferTraits.h"


namespace Graphics::D3D11 {
	class Driver;
	class Render;

	using ConstantBuffer = comptr<ID3D11Buffer>;
	using IndexBuffer = comptr<ID3D11Buffer>;
	using VertexBuffer = comptr<ID3D11Buffer>;
	using RawMemory = gsl::span<const std::byte>;

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
		ConstantBuffer createConstantBuffer(RawMemory memory) {
			return createBuffer(
				static_cast<void*>(const_cast<std::byte *>(memory.data())), memory.size_bytes(),
				D3D11_USAGE_DYNAMIC,
				D3D11_BIND_CONSTANT_BUFFER,
				D3D11_CPU_ACCESS_WRITE);
		}

		template<typename BufferTraitsT>
		VertexBuffer createVertexBuffer(RawMemory memory) {
			using BT = Graphics::D3D11::BufferTraits::BufferTraits< BufferTraitsT>;
			return createBuffer(static_cast<void*>(const_cast<std::byte *>(memory.data())), memory.size_bytes(),
				BT::Binding,
				D3D11_BIND_VERTEX_BUFFER,
				BT::CPU);
		}
		
		template <typename BufferTraitsT>
		IndexBuffer createIndexBuffer(RawMemory memory) {
			using BT = Graphics::D3D11::BufferTraits::BufferTraits< BufferTraitsT>;
			return createBuffer(static_cast<void*>(const_cast<std::byte *>(memory.data())), memory.size_bytes(),
				BT::Binding,
				D3D11_BIND_INDEX_BUFFER,
				BT::CPU);
		}

		

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

		std::unique_ptr<Render> getRender();

	protected:

		void setDefaultViewport();
		void setupRenderTargetView(); // Call First
		void setupDepthBuffer(); // Call 2nd
		void setupDepthStencil();
		void setupRasterDescription(); //Call 3rd

		ConstantBuffer createBuffer(void * mem, std::size_t memSize,
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
		

		friend class Graphics::D3D11::Render;
	};
	 

}