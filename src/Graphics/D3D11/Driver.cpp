#include "../../stdafx.h"
#include <d3d11.h>
#include "../DXGI/dxgi.h"
#include "Driver.h"

#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "D3DCompiler.lib")

// Comment out to prevent D3D11 Debug Runtime
#define DEBUGD3D11_RUNTIME


namespace Graphics::D3D11 {

	Driver::Driver(ComPtr<IDXGISwapChain> swap,
		ComPtr<ID3D11Device> device, 
		ComPtr<ID3D11DeviceContext> context, 
		const Graphics::Generic::DisplayMode mode) :
		mSwapChain(swap), mDevice(device), mRender(context), mMode(mode) {

	}

	void Driver::setupRenderTargetView() {
		// Get the pointer to the back buffer.
		ComPtr<ID3D11Texture2D> backbuffer;
		HRESULT result = mSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), &backbuffer);
		if (FAILED(result))
		{
			return;
		}

		// Create the render target view with the back buffer pointer.
		result = mDevice->CreateRenderTargetView(backbuffer.Get(), NULL, &mRenderTargetView);
		if (FAILED(result))
		{
			return;
		}
	}

	void Driver::setupDepthBuffer() {
		D3D11_TEXTURE2D_DESC depthBufferDesc;

		// Initialize the description of the depth buffer.
		ZeroMemory(&depthBufferDesc, sizeof(depthBufferDesc));

		// Set up the description of the depth buffer.
		depthBufferDesc.Width = mMode.width;
		depthBufferDesc.Height = mMode.height;
		depthBufferDesc.MipLevels = 1;
		depthBufferDesc.ArraySize = 1;
		depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		depthBufferDesc.SampleDesc.Count = 1;
		depthBufferDesc.SampleDesc.Quality = 0;
		depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		depthBufferDesc.CPUAccessFlags = 0;
		depthBufferDesc.MiscFlags = 0;

		// Create the texture for the depth buffer using the filled out description.
		HRESULT result = mDevice->CreateTexture2D(&depthBufferDesc, NULL, &mDepthStencilBuffer);
		if (FAILED(result))
		{
			return;
		}

	}

	void Driver::setupDepthStencil() {
		D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
		D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;

		// Initialize the description of the stencil state.
		ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));

		// Set up the description of the stencil state.
		depthStencilDesc.DepthEnable = true;
		depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;

		depthStencilDesc.StencilEnable = true;
		depthStencilDesc.StencilReadMask = 0xFF;
		depthStencilDesc.StencilWriteMask = 0xFF;

		// Stencil operations if pixel is front-facing.
		depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
		depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

		// Stencil operations if pixel is back-facing.
		depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
		depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

		// Create the depth stencil state.
		HRESULT result = mDevice->CreateDepthStencilState(&depthStencilDesc, mDepthStencilState.GetAddressOf());
		if (FAILED(result))
		{
			return;
		}

		// Set the depth stencil state.
		mRender->OMSetDepthStencilState(mDepthStencilState.Get(), 1);

		// Initailze the depth stencil view.
		ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));

		// Set up the depth stencil view description.
		depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		depthStencilViewDesc.Texture2D.MipSlice = 0;

		// Create the depth stencil view.
		result = mDevice->CreateDepthStencilView(mDepthStencilBuffer.Get(), &depthStencilViewDesc, &mDepthStencilView);
		if (FAILED(result))
		{
			return;
		}

		// Bind the render target view and depth stencil buffer to the output render pipeline.
		mRender.setDepthAndRenderTargetViews(mRenderTargetView, mDepthStencilView);
	}

	void Driver::setupRasterDescription() {
		D3D11_RASTERIZER_DESC rasterDesc;

		// Setup the raster description which will determine how and what polygons will be drawn.
		rasterDesc.AntialiasedLineEnable = false;
		rasterDesc.CullMode = D3D11_CULL_BACK;
		rasterDesc.DepthBias = 0;
		rasterDesc.DepthBiasClamp = 0.0f;
		rasterDesc.DepthClipEnable = true;
		rasterDesc.FillMode = D3D11_FILL_SOLID;
		rasterDesc.FrontCounterClockwise = false;
		rasterDesc.MultisampleEnable = false;
		rasterDesc.ScissorEnable = false;
		rasterDesc.SlopeScaledDepthBias = 0.0f;

		// Create the rasterizer state from the description we just filled out.
		HRESULT result = mDevice->CreateRasterizerState(&rasterDesc, &mRasterState);
		if (FAILED(result))
		{
			return;
		}

		// Now set the rasterizer state.
		mRender->RSSetState(mRasterState.Get());

		// Create the viewport.
		mRender.setViewPort((float)mMode.width, (float)mMode.height);
	}

	void Driver::setupDefaults() {
		setupRenderTargetView(); // Call First
		setupDepthBuffer(); // Call 2nd
		setupDepthStencil();
		setupRasterDescription(); //Call 3rd
	}

	void Driver::resize(unsigned int width, unsigned int height) {

		mRender->OMSetRenderTargets(0, 0, 0);

		// Preserve the existing buffer count and format.
		// Automatically choose the width and height to match the client rect for HWNDs.
		HRESULT hr = mSwapChain->ResizeBuffers(0, 0, 0, DXGI_FORMAT_UNKNOWN, 0);

		if (FAILED(hr)) {
			return;
		}

		// Get buffer and create a render-target-view.
		setupRenderTargetView();

		mRender->OMSetRenderTargets(1, mRenderTargetView.GetAddressOf(), NULL);

		mRender.setViewPort((float)width, (float)height);

	}

	void Driver::present() {
		// Present the back buffer to the screen since rendering is complete.
		if (mMode.useVsync)
		{
			// Lock to screen refresh rate.
			mSwapChain->Present(1, 0);
		}
		else
		{
			// Present as fast as possible.
			mSwapChain->Present(0, 0);
		}
	}

	ComPtr<ID3D11Buffer> Driver::createBuffer(void * mem, unsigned int memSize,
		D3D11_USAGE bufferMemoryType,
		unsigned int bindFlags,
		unsigned int CPUAccessFlags)
	{
		ID3D11Buffer * tempbuf = nullptr;
		D3D11_BUFFER_DESC vertexBufferDesc;
		D3D11_SUBRESOURCE_DATA vertexData;

		// Set up the description of the generic buffer.
		vertexBufferDesc.Usage = bufferMemoryType;
		vertexBufferDesc.ByteWidth = memSize;
		vertexBufferDesc.BindFlags = bindFlags;
		vertexBufferDesc.CPUAccessFlags = CPUAccessFlags;
		vertexBufferDesc.MiscFlags = 0;
		vertexBufferDesc.StructureByteStride = 0;

		// Give the subresource structure a pointer to the vertex data.
		vertexData.pSysMem = mem;
		vertexData.SysMemPitch = 0;
		vertexData.SysMemSlicePitch = 0;

		// Now create the vertex buffer.
		auto result = mDevice->CreateBuffer(&vertexBufferDesc, &vertexData, &tempbuf);
		if (FAILED(result))
		{
			return nullptr;
		}

		return { tempbuf };
	};
	

	std::unique_ptr<Graphics::Generic::Driver<Driver>> Driver::CreateDevice(HWND hwnd, 
		const Graphics::Generic::DisplayMode mode)
	{
		HRESULT result;
		DXGI_SWAP_CHAIN_DESC swapChainDesc;
		D3D_FEATURE_LEVEL featureLevel;
		ComPtr<IDXGISwapChain>			mSwapChain = nullptr;
		ComPtr<ID3D11Device>			mDevice = nullptr;
		ComPtr<ID3D11DeviceContext>		mDeviceContext = nullptr;


		// Initialize the swap chain description.
		ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));

		// Set to a single back buffer.
		swapChainDesc.BufferCount = 1;

		// Set the width and height of the back buffer.
		swapChainDesc.BufferDesc.Width = mode.width;
		swapChainDesc.BufferDesc.Height = mode.height;

		// Set regular 32-bit surface for the back buffer.
		swapChainDesc.BufferDesc.Format = DX::DXGI::ScreenFormat;


		// Set the refresh rate of the back buffer.
		if (mode.useVsync)
		{
			swapChainDesc.BufferDesc.RefreshRate.Numerator = mode.refreshrate.numerator;
			swapChainDesc.BufferDesc.RefreshRate.Denominator = mode.refreshrate.denominator;
		}
		else
		{
			swapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
			swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
		}

		// Set the usage of the back buffer.
		swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;

		// Set the handle for the window to render to.
		swapChainDesc.OutputWindow = hwnd;

		// Turn multisampling off.
		swapChainDesc.SampleDesc.Count = 1;
		swapChainDesc.SampleDesc.Quality = 0;

		// Set to full screen or windowed mode.
		if (mode.useFullscreen)
		{
			swapChainDesc.Windowed = false;
		}
		else
		{
			swapChainDesc.Windowed = true;
		}

		// Set the scan line ordering and scaling to unspecified.
		swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

		// Discard the back buffer contents after presenting.
		swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

		// Don't set the advanced flags.
		swapChainDesc.Flags = 0;

		// Set the feature level to DirectX 11.
		featureLevel = D3D_FEATURE_LEVEL_11_0;


#ifdef DEBUGD3D11_RUNTIME
		uint32_t creationFlags = D3D11_CREATE_DEVICE_DEBUG;
#else
		uint32_t creationFlags = 0;

#endif

		// Create the swap chain, Direct3D device, and Direct3D device context.
		result = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, creationFlags, &featureLevel, 1,
			D3D11_SDK_VERSION, &swapChainDesc, &mSwapChain, &mDevice, NULL, &mDeviceContext);
		if (FAILED(result))
		{
			throw;
		}

		return std::make_unique<Graphics::Generic::Driver<Driver>>(mSwapChain, mDevice, mDeviceContext, mode);
	}
}