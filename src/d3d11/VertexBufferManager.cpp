#include "../stdafx.h"
#include "BufferManagerHelpers.h"
#include "VertexBufferManager.h"
#include "../util/GenericHandleManager.h"

using namespace BufferHelpers;

std::vector< GenericHandleManager::ItemDescription<BufferItem> >	VertexBuffers;
std::vector< uint32_t >		VertexBuffersFreelist;
std::vector< HandleInvalidationCallback > VertexBuffersCallbacks;

void VertexBufferManager::executeCallbacks() 
{
	for( auto loop : VertexBuffersCallbacks )
	{
		loop();
	}
}

void VertexBufferManager::initalize(uint32_t expectedSize )
{
	VertexBuffers.reserve( expectedSize );
}

void VertexBufferManager::shutdown()
{
	// Do nothing for now
	//deleteAllBuffers();
}

VertexBufferHandle VertexBufferManager::newImmutable( D3D11 & d3d, void * verts, uint32_t memSize, uint32_t tag )
{
	BufferItem bItem;
	bItem.mData = D3DPointer<ID3D11Buffer>( createBuffer( d3d, verts, memSize, D3D11_USAGE_IMMUTABLE, D3D11_BIND_VERTEX_BUFFER ) );
	bItem.mTag = tag;
	VertexBufferHandle vHandle = GenericHandleManager::newHandle<VertexBufferHandle, BufferItem>( bItem, VertexBuffers, VertexBuffersFreelist );
	vHandle.mUserData = BindingTypes::Immutable;
	return vHandle;
}

VertexBufferHandle VertexBufferManager::newDynamic( D3D11 & d3d, void * verts, uint32_t memSize, uint32_t tag )
{
	BufferItem bItem;
	bItem.mData = D3DPointer<ID3D11Buffer>( createBuffer( d3d, verts, memSize, D3D11_USAGE_DYNAMIC, D3D11_BIND_VERTEX_BUFFER ) );
	bItem.mTag = tag;
	VertexBufferHandle vHandle = GenericHandleManager::newHandle<VertexBufferHandle, BufferItem>( bItem, VertexBuffers, VertexBuffersFreelist );
	vHandle.mUserData = BindingTypes::Dynamic;
	return vHandle;
}

VertexBufferHandle VertexBufferManager::newStatic( D3D11 & d3d, void * verts, uint32_t memSize, uint32_t tag )
{
	BufferItem bItem;
	bItem.mData = D3DPointer<ID3D11Buffer>( createBuffer( d3d, verts, memSize, D3D11_USAGE_DEFAULT, D3D11_BIND_VERTEX_BUFFER ) );
	bItem.mTag = tag;
	VertexBufferHandle vHandle = GenericHandleManager::newHandle<VertexBufferHandle, BufferItem>( bItem, VertexBuffers, VertexBuffersFreelist );
	vHandle.mUserData = BindingTypes::Static;
	return vHandle;
}

ID3D11Buffer * VertexBufferManager::fromHandle( VertexBufferHandle buffer )
{
	return GenericHandleManager::fromHandle( buffer, VertexBuffers ).mData;
}

void VertexBufferManager::editBuffer( D3D11 & d3d, VertexBufferHandle buffer, void * verts, uint32_t memSize )
{

#ifdef _DEBUG
	if( buffer.isNull() )
	{
		//Todo: Void log an error
		return;
	}


	if( buffer.mUserData == BindingTypes::Dynamic )
	{
		// Todo: log an error don't update immutable types
		return;
	}

#endif

	ID3D11Buffer * buf = fromHandle( buffer );

#ifdef _DEBUG

	if( memSize > getSizeOfBuffer( buf ) )
	{
		// todo: log an error size to big for current buffer
		return;
	}
#endif 

	mapBuffer( d3d, buf, verts, memSize, D3D11_MAP_WRITE_DISCARD );
}

void VertexBufferManager::deleteBuffer( VertexBufferHandle buffer )
{
	GenericHandleManager::deleteHandle( buffer, VertexBuffers, VertexBuffersFreelist );
	executeCallbacks();
}

void VertexBufferManager::deleteAllBuffers( )
{
	VertexBuffers.clear();
	VertexBuffersFreelist.clear();
	executeCallbacks();
}

void VertexBufferManager::deleteBuffersWithTag( uint32_t tag )
{
	for( uint32_t count = 0; count < VertexBuffers.size(); ++count )
	{
		auto & loop = VertexBuffers[count];
		if( loop.mValue.mTag == tag )
		{
			++loop.mGeneration;
			loop.mValue.mData = nullptr;
			VertexBuffersFreelist.push_back( count );
		}
	}
}


void VertexBufferManager::registerHandleInvalidation( HandleInvalidationCallback hicall )
{
	VertexBuffersCallbacks.push_back( hicall );
}