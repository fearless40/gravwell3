include "../../stdafx.h"
#include <d3d11.h>
#include "../DXGI/dxgi.h"
#include "../Shared.h"
#include "Driver.h"
#include "RenderQue.h"

namespace Graphics::D3D11 {

	struct PerGameConstantBuffer {
		DirectX::XMMATRIX projection;
	};


	RenderQue::RenderQue(Driver * device, comptr<ID3D11DeviceContext> context) :
		mDevice{ device },
		mRender{ context }
	{
	}

	void RenderQue::InitalizeQue() {
		PerGameConstantBuffer gCB;

		mPerGame = mDevice->createConstantBuffer(&gCB, sizeof(PerGameConstantBuffer));

	}


	

}