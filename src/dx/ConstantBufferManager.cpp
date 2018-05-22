#include "../stdafx.h"
#include "../util/TagArray.h"
#include "BufferManagerHelpers.h"
#include "ConstantBufferManager.h"
#include "../util/GenericHandleManager.h"

using namespace BufferHelpers;

std::vector< GenericHandleManager::ItemDescription<BufferItem> >	ConstantBuffers;
std::vector< uint32_t >		ConstantBuffersFreelist;
std::vector< HandleInvalidationCallback > ConstantBuffersCallbacks;
TagArray<ConstantBufferHandle, uint32_t> tags;

void ConstantBufferManager::executeCallbacks() 
{
	for( auto loop : ConstantBuffersCallbacks )
	{
		loop();
	}
}

void ConstantBufferManager::initalize(uint32_t expectedSize )
{
	ConstantBuffers.reserve( expectedSize );
}

void ConstantBufferManager::shutdown()
{
	// Do nothing for now
	//deleteAllBuffers();
}

/*ConstantBufferHandle ConstantBufferManager::newImmutable( D3D11 & d3d, void * verts, uint32_t memSize, uint32_t tag )
{
	BufferItem bItem;
	bItem.mData = D3DPointer<ID3D11Buffer>( createBuffer( d3d, verts, memSize, D3D11_USAGE_IMMUTABLE, D3D11_BIND_VERTEX_BUFFER ) );
	bItem.mTag = tag;
	ConstantBufferHandle vHandle = GenericHandleManager::newHandle<ConstantBufferHandle, BufferItem>( bItem, ConstantBuffers, ConstantBuffersFreelist );
	vHandle.mUserData = BindingTypes::Immutable;
	return vHandle;
}*/

ConstantBufferHandle ConstantBufferManager::newDynamic( D3D11 & d3d, void * verts, uint32_t memSize, uint32_t tag )
{
	BufferItem bItem;
	bItem.mData = D3DPointer<ID3D11Buffer>( createConstantBuffer( d3d, verts, memSize ) );
	bItem.mTag = tag;
	ConstantBufferHandle vHandle = GenericHandleManager::newHandle<ConstantBufferHandle, BufferItem>( bItem, ConstantBuffers, ConstantBuffersFreelist );
	vHandle.mUserData = BindingTypes::Dynamic;

	if( tag != 0 )
	{
		tags.add( tag, vHandle );
	}
	return vHandle;
}

/*ConstantBufferHandle ConstantBufferManager::newStatic( D3D11 & d3d, void * verts, uint32_t memSize, uint32_t tag )
{
	BufferItem bItem;
	bItem.mData = D3DPointer<ID3D11Buffer>( createBuffer( d3d, verts, memSize, D3D11_USAGE_DEFAULT, D3D11_BIND_VERTEX_BUFFER ) );
	bItem.mTag = tag;
	ConstantBufferHandle vHandle = GenericHandleManager::newHandle<ConstantBufferHandle, BufferItem>( bItem, ConstantBuffers, ConstantBuffersFreelist );
	vHandle.mUserData = BindingTypes::Static;
	return vHandle;
}*/

ID3D11Buffer * ConstantBufferManager::fromHandle( ConstantBufferHandle buffer )
{
	return GenericHandleManager::fromHandle( buffer, ConstantBuffers ).mData;
}

void ConstantBufferManager::editBuffer( D3D11 & d3d, ConstantBufferHandle buffer, void * verts, uint32_t memSize )
{

#ifdef _DEBUG
	if( buffer.isNull() )
	{
		//Todo: Void log an error
		return;
	}


	if( buffer.mUserData !=  BindingTypes::Dynamic )
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

void ConstantBufferManager::deleteBuffer( ConstantBufferHandle buffer )
{
	GenericHandleManager::deleteHandle( buffer, ConstantBuffers, ConstantBuffersFreelist );
	executeCallbacks();
}

void ConstantBufferManager::deleteAllBuffers( )
{
	ConstantBuffers.clear();
	ConstantBuffersFreelist.clear();
	executeCallbacks();
}

void ConstantBufferManager::deleteBuffersWithTag( uint32_t tag )
{
	auto deleteer = [] ( ConstantBufferHandle handle ) {
		GenericHandleManager::deleteHandle( handle, ConstantBuffers, ConstantBuffersFreelist );
	};

	tags.removeItemByTag( tag, deleteer );

	executeCallbacks();
	/*
	
	for( uint32_t count = 0; count < ConstantBuffers.size(); ++count )
	{
		auto & loop = ConstantBuffers[count];
		if( loop.mValue.mTag == tag )
		{
			++loop.mGeneration;
			loop.mValue.mData = nullptr;
			ConstantBuffersFreelist.push_back( count );
		}
	}
	*/
}


void ConstantBufferManager::registerHandleInvalidation( HandleInvalidationCallback hicall )
{
	ConstantBuffersCallbacks.push_back( hicall );
}

