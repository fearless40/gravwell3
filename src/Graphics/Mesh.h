#pragma once


#include "VertexTypeDescription.h"
#include "Handles.h"

struct ID3D11InputLayout;


// Public Facing struct for the user
struct Mesh
{
	static const int MAX_NUMBER_OF_BUFFERS = 4;

	D3D11_PRIMITIVE_TOPOLOGY	mTopology;
	ID3D11InputLayout		*	mLayout;
	VertexBufferHandle			mVertexBuffers[MAX_NUMBER_OF_BUFFERS];
	uint32_t					mStrides[MAX_NUMBER_OF_BUFFERS];		// Array of strides if multiple vertex buffers are used
	uint32_t					mVOffset[MAX_NUMBER_OF_BUFFERS];		// Start Vertex position in the VertexBuffer (should always be 0)
	IndexBufferHandle			mIndexBuffer;		
};

// Used internally to render items directly (saves a step of indirection of having to look up all the handles)
struct MeshCache
{
	static const uint32_t INDEX_BUFFER_POSITION = 0;
	static const uint32_t VERTEX_BUFFER_POSITION = 1;
	
	// Buffer 0 is always the index buffer
	// Buffer 1 is the start of the vertex buffers
	ID3D11Buffer			*	mBuffers[Mesh::MAX_NUMBER_OF_BUFFERS+1];
	uint32_t					mNbrOfVertexBuffers;
	ID3D11Buffer			*	getIndexBuffer() { return mBuffers[0]; }
	ID3D11Buffer			**	getVertexBuffers(){ return &mBuffers[1]; }

	D3D11_PRIMITIVE_TOPOLOGY	mTopology;
	ID3D11InputLayout		*	mLayout;
	uint32_t					mStrides[Mesh::MAX_NUMBER_OF_BUFFERS];		// Array of strides if multiple vertex buffers are used
	uint32_t					mVOffset[Mesh::MAX_NUMBER_OF_BUFFERS];		// Start Vertex position in the VertexBuffer (should always be 0)
};
