#include "../stdafx.h"
#include "BufferManagerHelpers.h"
#include "D3D11.h"

namespace BufferHelpers
{

ID3D11Buffer * createBuffer( D3D11 & d3d, void * mem, unsigned int memSize, D3D11_USAGE bufferMemoryType, unsigned int bindFlags )
{
	ID3D11Buffer * tempbuf = nullptr; 

	D3D11_BUFFER_DESC vertexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData;
	HRESULT result;

	// Set up the description of the static vertex buffer.
	vertexBufferDesc.Usage = bufferMemoryType;
	vertexBufferDesc.ByteWidth = memSize;
	vertexBufferDesc.BindFlags = bindFlags;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the vertex data.
	vertexData.pSysMem = mem;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	// Now create the vertex buffer.
	result = d3d.getDevice()->CreateBuffer(&vertexBufferDesc, &vertexData, &tempbuf);
	if(FAILED(result))
	{
		return nullptr;
	}

	return tempbuf;
}

uint32_t getSizeOfBuffer( ID3D11Buffer * buf )
{
	if( !buf )
		return 0;

	D3D11_BUFFER_DESC desc;

	buf->GetDesc( &desc );

	return desc.ByteWidth;
}

void mapBuffer( D3D11 & d3d, ID3D11Buffer * buf, void * data, uint32_t memSize, D3D11_MAP mapType )
{
	D3D11_MAPPED_SUBRESOURCE res;
	d3d.getDeviceContext()->Map( buf, 0, mapType, 0, &res );
	
	memcpy( res.pData, data, memSize );

	d3d.getDeviceContext()->Unmap( buf, 0 );
}


ID3D11Buffer * createConstantBuffer( D3D11 & d3d, void * data, unsigned int memSize )
{
	D3D11_BUFFER_DESC desc;
	D3D11_SUBRESOURCE_DATA dataBuffer;
	ID3D11Buffer * tempbuf = nullptr;

	desc.ByteWidth = memSize;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	desc.MiscFlags = 0;
	desc.StructureByteStride = 0;
	desc.Usage = D3D11_USAGE_DYNAMIC;
	desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

	dataBuffer.pSysMem = data;
	dataBuffer.SysMemPitch = 0;
	dataBuffer.SysMemSlicePitch = 0;

	
	HRESULT hr;
	hr = d3d.getDevice()->CreateBuffer( &desc, &dataBuffer, &tempbuf );

	if( FAILED(hr) )
	{
		return nullptr;
	}

	return tempbuf;
}
};