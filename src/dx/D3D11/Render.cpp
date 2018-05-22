#include "../../stdafx.h"
#include <d3d11.h>
#include "../Shared.h"
#include "Render.h"


namespace DX::D3D11 {

	Render::Render(ComPtr<ID3D11DeviceContext>	DeviceContext) :
		mDeviceContext(DeviceContext)
	{

	}

	void Render::setViewPort(float width, float height, float MinDepth, float MaxDepth, float TopLeftX, float TopLeftY) {
		D3D11_VIEWPORT viewport;

		// Setup the viewport for rendering.
		viewport.Width = width;
		viewport.Height = height;
		viewport.MinDepth = MinDepth;
		viewport.MaxDepth = MaxDepth;
		viewport.TopLeftX = TopLeftX;
		viewport.TopLeftY = TopLeftY;

		// Create the viewport.
		mDeviceContext->RSSetViewports(1, &viewport);
	}
}