
#pragma once

#include "Handles.h"



class IndexBufferManager
{
private:
	static void executeCallbacks(); 

public:
	static void initalize(uint32_t expectedSize = 1024);
	static void shutdown();

	static IndexBufferHandle newImmutable	( D3D11 & d3d, void * verts, uint32_t memSize, uint32_t tag = 0);
	static IndexBufferHandle newStatic		( D3D11 & d3d, void * verts, uint32_t memSize, uint32_t tag = 0);
	static IndexBufferHandle newDynamic	( D3D11 & d3d, void * verts, uint32_t memSize, uint32_t tag = 0);

	static void editBuffer( D3D11 & d3d, IndexBufferHandle buffer, void * verts, uint32_t memSize );

	static void	deleteBuffer( IndexBufferHandle buffer );

	static void deleteAllBuffers( );

	static void deleteBuffersWithTag( uint32_t tag );

	static void registerHandleInvalidation( HandleInvalidationCallback hicall );
	
	static ID3D11Buffer * fromHandle( IndexBufferHandle buffer );

};