#pragma once

struct ID3D11Buffer;
class D3D11;

#include "Handles.h"
#include "..\util\GenericHandleManager.h"
#include "D3DPointer.h"
#include <functional>




// Single Threaded access only no protection for multiple threads of access
class BufferManager
{
private:
	typedef VertexBufferHandle VertexBufferHandle;
	typedef IndexBufferHandle IndexBufferHandle;
	typedef ConstantBufferHandle ConstantBufferHandle; 


public:

	


	static VertexBufferHandle createVBImmutable( D3D11 & d3d, void * verts, unsigned int memSize  );

	static VertexBufferHandle createVBStatic( D3D11 & d3d, void * verts, unsigned int memSize  );

	static IndexBufferHandle createIBStatic( D3D11 & d3d, void * indexs, unsigned int memSize );

	static IndexBufferHandle createIBImmutable( D3D11 & d3d, void * indexs, unsigned int memSize );

	static ConstantBufferHandle createConstantBuffer( D3D11 & d3d, void * data, unsigned int memSize );

	static ID3D11Buffer * getVertexBuffer( VertexBufferHandle vbuffer ) 
	{
#ifdef _DEBUG
		if( vbuffer.mGeneration != mVertexArray[vbuffer.mIndex].mCount )
		{
			// Todo: STOP the program
		}
#endif

		return mVertexArray[vbuffer.mIndex].mBuffer;
	}

	static ID3D11Buffer * getIndexBuffer( IndexBufferHandle ibuffer ) 
	{
#ifdef _DEBUG
		if( ibuffer.mGeneration != mIndexArray[ibuffer.mIndex].mCount )
		{
			// Todo: STOP the program
		}
#endif

		return mIndexArray[ibuffer.mIndex].mBuffer;
	}

	static ID3D11Buffer * getConstantBuffer( ConstantBufferHandle cbuffer )
	{
#ifdef _DEBUG
		if( cbuffer.mGeneration != mConstantArray[cbuffer.mIndex].mCount )
		{
			// Todo: STOP the program
		}
#endif
		return mConstantArray[cbuffer.mIndex].mBuffer;
	}

	static ID3D11Buffer ** getBindableConstantBuffer( ConstantBufferHandle cbuffer )
	{
		#ifdef _DEBUG
		if( cbuffer.mGeneration != mConstantArray[cbuffer.mIndex].mCount )
		{
			// Todo: STOP the program
		}
		#endif

		return &mConstantArray[cbuffer.mIndex].mBuffer;
	}

	static bool isValidConstantBufferHandle( ConstantBufferHandle cbuffer );
	static bool isValidVertexBufferHandle( VertexBufferHandle cbuffer );
	static bool isValidIndexBufferHandle( IndexBufferHandle cbuffer );

	static void freeAllBuffers();
	static void freeVertexBuffers();
	static void freeIndexBuffers();
	static void freeConstantBuffers();

protected:

	static const int MAX_BUFFERS = 1024;
	static const int MAX_CONSTANT_BUFFERS = MAX_BUFFERS * 2;

	struct buffer {
		unsigned short mCount;
		ID3D11Buffer * mBuffer;
	};

	/*typedef std::vector< buffer > BufferArray;

	BufferArray mVertexArray;
	BufferArray mIndexArray;
	*/
	// The last item in the array
	static int32_t mVertexNext, mIndexNext, mConstantNext;
	static buffer mVertexArray[MAX_BUFFERS];
	static buffer mIndexArray[MAX_BUFFERS];
	static buffer mConstantArray[MAX_CONSTANT_BUFFERS];
};

