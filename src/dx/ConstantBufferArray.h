#pragma once


#include "Handles.h"

struct ID3D11Buffer;

/*
class BufferArray 
{
public:
	BufferArray(void);
	~BufferArray(void);

	BufferArray( const BufferArray & ba );
	
	void setNumberOfBuffers( unsigned int buffNbr );
	unsigned int getNumberOfBuffers() const { return mNbrBuffers; } 

	void setBuffer( ID3D11Buffer * buf, unsigned int index );
	ID3D11Buffer * getBuffer( unsigned int index );

	// Does not change the number of elemnts in the buffer array.
	// just delete the single buffer entry. Becareful with this function
	// void eraseBuffer( unsigned int index  );

	bool isEmpty() const { return ( mNbrBuffers == 0 ? false : true); }

	ID3D11Buffer ** getBuffers() const { return mBuffers; } 

	// Bind location (useful for when other buffers are already bound to the stream)
	unsigned short		mStartBindLocation;

protected:
	// C++ Array of buffers
	ID3D11Buffer	**	mBuffers;

	// Number of buffers in the array
	unsigned short		mNbrBuffers;

	
};*/


class ConstantBufferArray
{
private:
	ConstantBufferArrayHandle	mHandle;
	uint16_t					mIndex;
	

public:
	uint16_t					mBindPosition;
	

public:
	void editBuffer( D3D11 & d3d, uint32_t index, void * data, uint32_t memSize );
};

