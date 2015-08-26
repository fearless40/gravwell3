#include "../stdafx.h"
#include <d3d11.h>
#include "D3D11.h"
#include "BufferManager.h"





int32_t BufferManager::mVertexNext		= -1;
int32_t BufferManager::mIndexNext		= -1;
int32_t BufferManager::mConstantNext	= -1;

BufferManager::buffer BufferManager::mVertexArray[MAX_BUFFERS];
BufferManager::buffer BufferManager::mIndexArray[MAX_BUFFERS];
BufferManager::buffer BufferManager::mConstantArray[MAX_CONSTANT_BUFFERS];

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

BufferManager::VertexBufferHandle BufferManager::createVBImmutable( D3D11 & d3d, void * verts, unsigned int memSize  )
{
	mVertexArray[++mVertexNext].mCount++;
	mVertexArray[mVertexNext].mBuffer = createBuffer( d3d, verts, memSize, D3D11_USAGE_IMMUTABLE, D3D11_BIND_VERTEX_BUFFER );
	return VertexBufferHandle(mVertexArray[mVertexNext].mCount, mVertexNext);
}

BufferManager::VertexBufferHandle BufferManager::createVBStatic( D3D11 & d3d, void * verts, unsigned int memSize  )
{
	mVertexArray[++mVertexNext].mCount++;
	mVertexArray[mVertexNext].mBuffer = createBuffer( d3d, verts, memSize, D3D11_USAGE_DEFAULT, D3D11_BIND_VERTEX_BUFFER );
	return VertexBufferHandle(mVertexArray[mVertexNext].mCount, mVertexNext);
}

BufferManager::IndexBufferHandle BufferManager::createIBStatic( D3D11 & d3d, void * indexs, unsigned int memSize )
{
	mIndexArray[++mIndexNext].mCount++;
	mIndexArray[mIndexNext].mBuffer = createBuffer( d3d, indexs, memSize, D3D11_USAGE_DEFAULT, D3D11_BIND_INDEX_BUFFER );
	return IndexBufferHandle( mIndexArray[mIndexNext].mCount, mIndexNext );
}

BufferManager::IndexBufferHandle BufferManager::createIBImmutable( D3D11 & d3d, void * indexs, unsigned int memSize )
{
	mIndexArray[++mIndexNext].mCount++;
	mIndexArray[mIndexNext].mBuffer = createBuffer( d3d, indexs, memSize, D3D11_USAGE_IMMUTABLE, D3D11_BIND_INDEX_BUFFER );
	return IndexBufferHandle( mIndexArray[mIndexNext].mCount, mIndexNext );
}

BufferManager::ConstantBufferHandle BufferManager::createConstantBuffer( D3D11 & d3d, void * data, unsigned int memSize )
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
		return ConstantBufferHandle();
	}

	mConstantArray[++mConstantNext].mCount++;
	mConstantArray[mConstantNext].mBuffer = tempbuf;
	return IndexBufferHandle( mConstantArray[mConstantNext].mCount, mConstantNext );
}

bool BufferManager::isValidConstantBufferHandle( BufferManager::ConstantBufferHandle cbuffer )
{
	
	if( cbuffer.isNull() )
		return false;

	if( cbuffer.mIndex > MAX_CONSTANT_BUFFERS )
		return false;

	if( cbuffer.mGeneration != mConstantArray[cbuffer.mIndex].mCount )
		return false;

	return true;
}

bool BufferManager::isValidVertexBufferHandle( BufferManager::VertexBufferHandle cbuffer )
{
	if( cbuffer.isNull() )
		return false;

	if( cbuffer.mIndex > MAX_BUFFERS )
		return false;

	if( cbuffer.mGeneration != mVertexArray[cbuffer.mIndex].mCount )
		return false;

	return true;
}

bool BufferManager::isValidIndexBufferHandle( BufferManager::IndexBufferHandle cbuffer )
{
	if( cbuffer.isNull() )
		return false;

	if( cbuffer.mIndex > MAX_BUFFERS )
		return false;

	if( cbuffer.mGeneration != mIndexArray[cbuffer.mIndex].mCount )
		return false;

	return true;
}


void BufferManager::freeAllBuffers() {
	freeConstantBuffers();
	freeIndexBuffers();
	freeVertexBuffers();
}

void BufferManager::freeVertexBuffers()
{
	for( auto it : mVertexArray )
	{
		++it.mCount;
		if( it.mBuffer )
			it.mBuffer->Release();
	}
}
void BufferManager::freeIndexBuffers()
{
	for( auto it : mIndexArray )
	{
		++it.mCount;
		if( it.mBuffer )
			it.mBuffer->Release();
	}
}
void BufferManager::freeConstantBuffers()
{
	for( auto it : mConstantArray )
	{
		++it.mCount;
		if( it.mBuffer )
			it.mBuffer->Release();
	}
}