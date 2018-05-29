#pragma once

#include "../Generic/Color.h"

namespace Graphics::D3D11 {
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
			mDeviceContext->ClearRenderTargetView(mTargetView.Get(), color.data());
		}

		void clearDepthStencilTarget() {
			mDeviceContext->ClearDepthStencilView(mStencilView.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0);
		}

		ID3D11DeviceContext * get() { return mDeviceContext.Get(); }

		ID3D11DeviceContext * operator -> () { return mDeviceContext.Get(); }

	private:
		ComPtr<ID3D11DepthStencilView>  mStencilView = nullptr;
		ComPtr<ID3D11RenderTargetView>	mTargetView = nullptr;

		ComPtr<ID3D11DeviceContext>		mDeviceContext = nullptr;
	};
}