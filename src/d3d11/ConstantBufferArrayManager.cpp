#include "../stdafx.h"
#include <d3d11.h>
#include "D3D11.h"
#include "ConstantBufferManager.h"
#include "ConstantBufferArrayManager.h"
#include "../util/GenericHandleManager.h"



// GenericHandle3.mUserData = nbr of buffers used
// GenericHandle3.mIndex	= first occurence of said buffer

struct InternalBuffer
{
	uint16_t	mNbrBuffers;
	uint16_t	mStartIndex;
	uint16_t	mBindLocation;
};

std::vector< GenericHandleManager::ItemDescription<InternalBuffer> >				BufferArray;
std::vector< ConstantBufferHandle >			ConstantHandles;
std::vector< ID3D11Buffer * >				CacheBuffers;
std::vector< uint32_t >						FreeItems;



void ConstantBufferArrayManager::rebuildCache()
{
	CacheBuffers.clear();
	for( auto handle : ConstantHandles )
	{
		CacheBuffers.push_back( ConstantBufferManager::fromHandle( handle ) );
	}
}

void ConstantBufferArrayManager::initalize(uint32_t nbrBufferArrays, uint32_t EstimatedNbrBuffersPerArray )
{
	BufferArray.reserve( nbrBufferArrays );
	ConstantHandles.reserve( nbrBufferArrays * EstimatedNbrBuffersPerArray );
	CacheBuffers.reserve( nbrBufferArrays * EstimatedNbrBuffersPerArray );

	ConstantBufferManager::registerHandleInvalidation( rebuildCache );
}


void ConstantBufferArrayManager::shutdown()
{

}



void ConstantBufferArrayManager::deleteHandle( ConstantBufferArrayHandle buf )
{
	GenericHandleManager::deleteHandle( buf, BufferArray, FreeItems );
}

ConstantBufferArrayHandle ConstantBufferArrayManager::newBufferArray( uint32_t nbrBuffers, uint32_t startBindPos )
{
	ConstantBufferArrayHandle newHandle;
	
	// Search for an empty spot first

	if( FreeItems.size() > 0 )
	{
		for( uint32_t search = 0 ; search < FreeItems.size(); ++search )
		{
			if( BufferArray[FreeItems[search]].mValue.mNbrBuffers == nbrBuffers )
			{
				auto & temp = BufferArray[FreeItems[search]];
				temp.mValue.mBindLocation = startBindPos;
				++temp.mGeneration;

				newHandle.mGeneration = temp.mGeneration;
				newHandle.mIndex = FreeItems[search] + 1;
				
				// Erase the item from the FreeItems list
				std::swap( FreeItems[search], FreeItems.back() );
				FreeItems.pop_back();
				return newHandle;
			}
		}
	}
	
	// If no empty spot found then generate a new index
	GenericHandleManager::ItemDescription<InternalBuffer> item = { 1, { nbrBuffers, ConstantHandles.size(), startBindPos } };
	BufferArray.push_back( item );

	for( uint32_t count = 0; count < nbrBuffers; ++count )
	{
		// Allocate the data needed for the buffer array
		ConstantHandles.push_back( ConstantBufferHandle() );
		CacheBuffers.push_back( nullptr );
	}

	
	return ConstantBufferArrayHandle( 1, std::distance( BufferArray.begin(), BufferArray.end() ));
}

bool ConstantBufferArrayManager::isValidHandle( ConstantBufferArrayHandle buffer )
{
	return GenericHandleManager::isValidHandle( buffer, BufferArray );
}

void ConstantBufferArrayManager::deleteAll()
{
	BufferArray.clear();
	ConstantHandles.clear();
	CacheBuffers.clear();
	FreeItems.clear();
}


void  ConstantBufferArrayManager::editBuffer( D3D11 & d3d, ConstantBufferArrayHandle buffer, uint32_t index, void * data, uint32_t memSize )
{
#ifdef _DEBUG
	if( !isValidHandle( buffer ) )
	{
		// Todo log it
		return;
	}
#endif

	auto & item = BufferArray[buffer.mIndex - 1].mValue;

#ifdef _DEBUG
	if( index > item.mNbrBuffers )
	{
		// Todo log the error
		return;
	}
#endif

	ConstantBufferManager::editBuffer( d3d, ConstantHandles[item.mStartIndex + index], data, memSize );
}

void  ConstantBufferArrayManager::setStartBindingLocation( ConstantBufferArrayHandle buffer, uint16_t bindLocation )
{
#ifdef _DEBUG
	if( !isValidHandle( buffer ) )
	{
		// Todo log it
		return;
	}
#endif

	BufferArray[buffer.mIndex - 1].mValue.mBindLocation = bindLocation;
}

void  ConstantBufferArrayManager::addDynamicBuffer( D3D11 & d3d, ConstantBufferArrayHandle buffer, uint32_t index, void * data, uint32_t memSize )
{
	#ifdef _DEBUG
	if( !isValidHandle( buffer ) )
	{
		// Todo log it
		return;
	}
#endif

	auto & item = BufferArray[buffer.mIndex-1].mValue;

#ifdef _DEBUG
	if( index > item.mNbrBuffers )
	{
		// Todo log the error
		return;
	}
#endif

	ConstantBufferHandle cbh = ConstantBufferManager::newDynamic( d3d, data, memSize );

	ConstantHandles[ index + item.mStartIndex ] = cbh;
	CacheBuffers[ index + item.mStartIndex ] = ConstantBufferManager::fromHandle( cbh );
}

uint32_t ConstantBufferArrayManager::getStartBindingLocation( ConstantBufferArrayHandle buffer )
{
#ifdef _DEBUG
	if( !isValidHandle( buffer ) )
	{
		// Todo log it
		return 0;
	}
#endif

	return BufferArray[ buffer.mIndex - 1 ].mValue.mBindLocation;
}

ID3D11Buffer ** ConstantBufferArrayManager::getBindableBuffers( ConstantBufferArrayHandle buffer )
{
#ifdef _DEBUG
	if( !isValidHandle( buffer ) )
	{
		// Todo log it
		return nullptr;
	}
#endif
		auto & item = BufferArray[buffer.mIndex - 1].mValue;

	return &CacheBuffers[ item.mStartIndex ];
}

uint32_t	ConstantBufferArrayManager::getNbrBuffers( ConstantBufferArrayHandle buffer )
{
	#ifdef _DEBUG
	if( !isValidHandle( buffer ) )
	{
		// Todo log it
		return 0;
	}
#endif
		return BufferArray[buffer.mIndex - 1].mValue.mNbrBuffers;
}

