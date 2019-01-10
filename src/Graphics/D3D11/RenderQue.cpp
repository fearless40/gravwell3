#include "../../stdafx.h"
#include <d3d11.h>
#include "../DXGI/dxgi.h"
#include "../Shared.h"
#include "Driver.h"
#include "RenderQue.h"

namespace Graphics::D3D11 {

	struct PerGameConstantBuffer {
		DirectX::XMFLOAT4X4 projection;
	};

	struct PerFrameConstantBUffer {
		DirectX::XMFLOAT4X4 worldView;
		float timeSinceLastFrame;
		float timeSinceStart;
		float blank1;
		float blank2;
		DirectX::XMFLOAT4 LightPosition;
		DirectX::XMFLOAT4 LightColor;
	};

	struct PerItemCB_Vertex {
		DirectX::XMFLOAT4 position;
		DirectX::XMFLOAT4 rotation;
	};

	RenderQue::RenderQue(Driver * device, comptr<ID3D11DeviceContext> context) :
		mDevice{ device },
		mRender{ context }
	{
	}

	void RenderQue::InitalizeQue() {
		PerGameConstantBuffer gCB;
		PerFrameConstantBUffer fCB;

		mPerGame = mDevice->createConstantBuffer(&gCB, sizeof(PerGameConstantBuffer));
		mPerFrame = mDevice->createConstantBuffer(&fCB, sizeof(PerFrameConstantBUffer));
		
	}


	

}