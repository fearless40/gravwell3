#include "../stdafx.h"
#include "BufferManagerHelpers.h"
#include "IndexBufferManager.h"
#include "../util/GenericHandleManager.h"

using namespace BufferHelpers;

std::vector< GenericHandleManager::ItemDescription<BufferItem> >	IndexBuffers;
std::vector< uint32_t >		IndexBuffersFreelist;
std::vector< HandleInvalidationCallback > IndexBuffersCallbacks;

void IndexBufferManager::executeCallbacks() 
{
	for( auto loop : IndexBuffersCallbacks )
	{
		loop();
	}
}

void IndexBufferManager::initalize(uint32_t expectedSize )
{
	IndexBuffers.reserve( expectedSize );
}

void IndexBufferManager::shutdown()
{
	// Do nothing for now
	//deleteAllBuffers();
}

IndexBufferHandle IndexBufferManager::newImmutable( D3D11 & d3d, void * verts, uint32_t memSize, uint32_t tag )
{
	BufferItem bItem;
	bItem.mData = D3DPointer<ID3D11Buffer>( createBuffer( d3d, verts, memSize, D3D11_USAGE_IMMUTABLE, D3D11_BIND_INDEX_BUFFER ) );
	bItem.mTag = tag;
	IndexBufferHandle vHandle = GenericHandleManager::newHandle<IndexBufferHandle, BufferItem>( bItem, IndexBuffers, IndexBuffersFreelist );
	vHandle.mUserData = BindingTypes::Immutable;
	return vHandle;
}

IndexBufferHandle IndexBufferManager::newDynamic( D3D11 & d3d, void * verts, uint32_t memSize, uint32_t tag )
{
	BufferItem bItem;
	bItem.mData = D3DPointer<ID3D11Buffer>( createBuffer( d3d, verts, memSize, D3D11_USAGE_DYNAMIC, D3D11_BIND_INDEX_BUFFER ) );
	bItem.mTag = tag;
	IndexBufferHandle vHandle = GenericHandleManager::newHandle<IndexBufferHandle, BufferItem>( bItem, IndexBuffers, IndexBuffersFreelist );
	vHandle.mUserData = BindingTypes::Dynamic;
	return vHandle;
}

IndexBufferHandle IndexBufferManager::newStatic( D3D11 & d3d, void * verts, uint32_t memSize, uint32_t tag )
{
	BufferItem bItem;
	bItem.mData = D3DPointer<ID3D11Buffer>( createBuffer( d3d, verts, memSize, D3D11_USAGE_DEFAULT, D3D11_BIND_INDEX_BUFFER ) );
	bItem.mTag = tag;
	IndexBufferHandle vHandle = GenericHandleManager::newHandle<IndexBufferHandle, BufferItem>( bItem, IndexBuffers, IndexBuffersFreelist );
	vHandle.mUserData = BindingTypes::Static;
	return vHandle;
}

ID3D11Buffer * IndexBufferManager::fromHandle( IndexBufferHandle buffer )
{
	return GenericHandleManager::fromHandle( buffer, IndexBuffers ).mData;
}

void IndexBufferManager::editBuffer( D3D11 & d3d, IndexBufferHandle buffer, void * verts, uint32_t memSize )
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

void IndexBufferManager::deleteBuffer( IndexBufferHandle buffer )
{
	GenericHandleManager::deleteHandle( buffer, IndexBuffers, IndexBuffersFreelist );
	executeCallbacks();
}

void IndexBufferManager::deleteAllBuffers( )
{
	IndexBuffers.clear();
	IndexBuffersFreelist.clear();
	executeCallbacks();
}

void IndexBufferManager::deleteBuffersWithTag( uint32_t tag )
{
	for( uint32_t count = 0; count < IndexBuffers.size(); ++count )
	{
		auto & loop = IndexBuffers[count];
		if( loop.mValue.mTag == tag )
		{
			++loop.mGeneration;
			loop.mValue.mData = nullptr;
			IndexBuffersFreelist.push_back( count );
		}
	}
}


void IndexBufferManager::registerHandleInvalidation( HandleInvalidationCallback hicall )
{
	IndexBuffersCallbacks.push_back( hicall );
}