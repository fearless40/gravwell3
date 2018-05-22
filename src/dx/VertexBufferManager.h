
#pragma once

#include "Handles.h"



class VertexBufferManager
{
private:
	static void executeCallbacks();

public:
	static void initalize(uint32_t expectedSize = 1024);
	static void shutdown();

	static VertexBufferHandle newImmutable	( D3D11 & d3d, void * verts, uint32_t memSize, uint32_t tag = 0);
	static VertexBufferHandle newStatic		( D3D11 & d3d, void * verts, uint32_t memSize, uint32_t tag = 0);
	static VertexBufferHandle newDynamic	( D3D11 & d3d, void * verts, uint32_t memSize, uint32_t tag = 0);

	static void editBuffer( D3D11 & d3d, VertexBufferHandle buffer, void * verts, uint32_t memSize );

	static void	deleteBuffer( VertexBufferHandle buffer );

	static void deleteAllBuffers( );

	static void deleteBuffersWithTag( uint32_t tag );

	static void registerHandleInvalidation( HandleInvalidationCallback hicall );
	
	static ID3D11Buffer * fromHandle( VertexBufferHandle buffer );

};