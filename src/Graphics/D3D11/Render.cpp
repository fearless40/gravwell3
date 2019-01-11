#include "../../stdafx.h"
#include <d3d11.h>
#include "../DXGI/dxgi.h"
#include "../Shared.h"
#include "Driver.h"
#include "Render.h"
#include "../Generic/Color.h"
#include "Types.h"

namespace Graphics::D3D11 {


	namespace impl {
		inline void writeToD3DBuffer(ID3D11DeviceContext*context, ID3D11Resource * resource, void const * data, std::size_t size) {
			D3D11_MAPPED_SUBRESOURCE rawPtr;
			context->Map(resource, 0, D3D11_MAP_WRITE_DISCARD, 0, &rawPtr);
			memcpy(rawPtr.pData, data, size);
			context->Unmap(resource, 0);
		}
	}

	template <typename T>
	void writeToD3DBuffer(ID3D11DeviceContext*context, ID3D11Resource * resource, T const & item) {
		impl::writeToD3DBuffer(context, resource, static_cast<void const*>(&item), sizeof(T));
	}

	struct PerGameConstantBuffer {
		DirectX::XMFLOAT4X4 projection;
		DirectX::XMFLOAT4 LightPosition;
		DirectX::XMFLOAT4 LightColor;
	};

	struct PerFrameConstantBuffer {
		float timeSinceLastFrame;
		float timeSinceStart;
		float blank1;
		float blank2;
	};

	struct PerItemConstantBuffer{
		DirectX::XMFLOAT4X4 worldView;
	};

	Render::Render(Driver * device, comptr<ID3D11DeviceContext> context) :
		mDevice{ device },
		mRender{ context }
	{
	}

	void Render::InitalizeQue() {
		PerGameConstantBuffer gCB;
		PerFrameConstantBuffer fCB;

		mPerGame = mDevice->createConstantBuffer(&gCB, sizeof(PerGameConstantBuffer));
		mPerFrame = mDevice->createConstantBuffer(&fCB, sizeof(PerFrameConstantBuffer));
		
	}

	void Render::setProjection(DirectX::XMFLOAT4X4 matrix) {
		mUpdatePerGame = true;
		mProjection = matrix;
	}

	void Render::setGobalLight(DirectX::XMFLOAT4 position, DirectX::XMFLOAT4 colors) {
		mUpdatePerGame = true;
		mGlobalLight.position = position;
		mGlobalLight.color = colors;
	}

	void Render::begin(float TimeSinceLastFrame, float TimeSinceStart) {
		if (mUpdatePerGame) {
			PerGameConstantBuffer  pgcb{ mProjection, mGlobalLight.position, mGlobalLight.color };
			writeToD3DBuffer(mRender.get(), mPerGame.get(), pgcb);
		}

		PerFrameConstantBuffer pfcb{ TimeSinceLastFrame, TimeSinceStart, 0.f,0.f };
		writeToD3DBuffer(mRender.get(), mPerFrame.get(), pfcb);
	}

	void Render::clear() {
		float color[4] = { 0.0f,0.0f,0.0f,0.0f };

		// Clear the back buffer.
		mRender->ClearRenderTargetView(mDevice->mRenderTargetView.get(), color);

		// Clear the depth buffer.
		mRender->ClearDepthStencilView(mDevice->mDepthStencilView.get(), D3D11_CLEAR_DEPTH, 1.0f, 0);
	}

	void Render::clear(Graphics::Generic::RGBA color) {
		// Clear the back buffer.
		mRender->ClearRenderTargetView(mDevice->mRenderTargetView.get(), color.cdata());

		// Clear the depth buffer.
		mRender->ClearDepthStencilView(mDevice->mDepthStencilView.get(), D3D11_CLEAR_DEPTH, 1.0f, 0);

	}

	void Render::drawMesh(DirectX::XMFLOAT4X4 worldview) {
		PerItemConstantBuffer picb{ worldview };
		writeToD3DBuffer(mRender.get(), mPerItem.get(), picb);
		mRender->DrawIndexed(mIndecies_count, mIndecies_start, 0);
	}
	
	void Render::end(bool autoPresent) {
		if (autoPresent)
			mDevice->present();
	}
}