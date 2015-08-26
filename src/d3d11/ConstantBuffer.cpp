#include "../stdafx.h"
#include "D3D11.h"
#include "BufferHelpers.h"
#include "ConstantBuffer.h"



ConstantBuffer::ConstantBuffer(void) 
{
#ifdef _DEBUG
	mSize = 0;
#endif
}


ConstantBuffer::~ConstantBuffer(void)
{
	
}

ConstantBuffer::ConstantBuffer( const ConstantBuffer & cb ) 
{
	mHandle = cb.mHandle;
#ifdef _DEBUG
	mSize = cb.mSize;
#endif
}



ConstantBuffer::ConstantBuffer(BufferManager::ConstantBufferHandle buf)
{
	setBuffer( buf );
}

void ConstantBuffer::setBuffer(BufferManager::ConstantBufferHandle buf)
{
#ifdef _DEBUG
	mSize = BufferHelpers::getSizeOfBuffer(BufferManager::getConstantBuffer( buf ) );
#endif
	mHandle = buf;
}

void ConstantBuffer::create(D3D11 * d3d, void * data, unsigned int size)
{
	mHandle = BufferManager::createConstantBuffer( d3d, data, size );

#ifdef _DEBUG
	mSize = size;
#endif
}

void ConstantBuffer::edit(D3D11 * d3d, void * data, unsigned int size )
{
#ifdef _DEBUG
	if( size > mSize ) {
		return;
	}

	if( !BufferManager::isValidConstantBufferHandle( mHandle ) )
		return;

#endif

	D3D11_MAPPED_SUBRESOURCE res;
	ID3D11Buffer * tbuffer = BufferManager::getConstantBuffer(mHandle);
	d3d->getDeviceContext()->Map( tbuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &res );
	
	memcpy( res.pData, data, size );

	d3d->getDeviceContext()->Unmap( tbuffer, 0 );

}


