#pragma once

#include "../Generic/Color.h"

namespace Graphics::D3D11 {
	struct VertexShaderPipelineTraits {
		using setConstantBuffer = Context::VSsetConstantBuffer;

	};
	
	
	class Context {
	public:
		Context(ComPtr<ID3D11DeviceContext>	DeviceContext);

		void setViewPort(float width, float height, 
			float MinDepth = 0.0f, float MaxDepth = 1.0f, 
			float TopLeftX = 0.0f, float TopLeftY = 0.0f);

		void setDepthAndRenderTargetViews(ComPtr<ID3D11RenderTargetView> TargetView,
			ComPtr<ID3D11DepthStencilView> StencilView)
		{
			mStencilView = StencilView;
			mTargetView = TargetView;
			mDeviceContext->OMSetRenderTargets(1, mTargetView.GetAddressOf(), mStencilView.Get());
		}

		void clearRenderTarget() {
			float color[4] = { 0.0f,0.0f,0.0f,1.0f };
			mDeviceContext->ClearRenderTargetView(mTargetView.Get(), color);
		}

		void clearRenderTarget(const Graphics::Generic::RGBA & color) {
			mDeviceContext->ClearRenderTargetView(mTargetView.Get(), color.cdata());
		}

		void clearDepthStencilTarget() {
			mDeviceContext->ClearDepthStencilView(mStencilView.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0);
		}

		void writeConstantBuffer(D3D11::Buffer & buf, void * mem, std::size_t size) {
			mDeviceContext->UpdateSubresource(buf.get(), 0, nullptr, mem, size, 0);
		}

		void VSsetConstantBuffer(D3D11::Buffer & buf, uint32_t startslot, uint32_t numbbuffers) {
			mDeviceContext->VSSetConstantBuffers(startslot, numbbuffers, buf.get());
		}

		ID3D11DeviceContext * get() { return mDeviceContext.Get(); }

		ID3D11DeviceContext * operator -> () { return mDeviceContext.Get(); }

	private:
		ComPtr<ID3D11DepthStencilView>  mStencilView = nullptr;
		ComPtr<ID3D11RenderTargetView>	mTargetView = nullptr;

		ComPtr<ID3D11DeviceContext>		mDeviceContext = nullptr;
	};
}