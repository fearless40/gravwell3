#include "../../stdafx.h"
#include <d3d11.h>
#include "../DXGI/dxgi.h"
#include "../Shared.h"
#include "Driver.h"
#include "Render.h"
#include "../Generic/Color.h"
#include "Types.h"

namespace Graphics::D3D11 {

	comptr<ID3D11Buffer> Render::createBuffer(void * mem, std::size_t memSize,
		D3D11_USAGE bufferMemoryType,
		unsigned int bindFlags,
		unsigned int CPUAccessFlags)
	{
		comptr<ID3D11Buffer> tempbuf{ nullptr };
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
		auto result = mDevice->get()->CreateBuffer(&vertexBufferDesc, &vertexData, tempbuf.put());
		if (FAILED(result))
		{
			return nullptr;
		}

		return  tempbuf;
	};
}