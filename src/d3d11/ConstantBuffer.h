#pragma once

struct ID3D11Buffer;
class D3D11;

#include "BufferManager.h"

class ConstantBuffer
{
protected:
	ConstantBufferHandle mHandle;
	
#ifdef _DEBUG
	unsigned int	mSize;
#endif

public:
	ConstantBuffer(void);
	~ConstantBuffer(void);
	ConstantBuffer(Buffers::ConstantBufferHandle buf);

	ConstantBuffer( const ConstantBuffer & cb );

	void setBuffer( Buffers::ConstantBufferHandle buf );

	void create(D3D11 * d3d,void * data, unsigned int size);

	template<typename T> void createSingle(D3D11 * d3d,T * data) {
		create( d3d, (void*) data, sizeof(T) );
	}

	template<typename T> void createArray(D3D11 * d3d,T * data, int nbrItems) {
		create( d3d, (void*) data, sizeof(data) * nbrItems );
	}

	void edit( D3D11 * d3d,  void * data, unsigned int size );

	template<typename T> void editSingle(D3D11 * d3d, T * data) {
		edit(d3d, (void*) data, sizeof(T) );
	}

	template<typename T> void editArray(D3D11 * d3d, T * data, int nbrItems) {
		edit( d3d, (void*) data, sizeof(data) * nbrItems );
	}

#ifdef _DEBUG
	unsigned int getSize() const { return mSize; }
#endif
	

	ID3D11Buffer * getBuffer() { return BufferManager::getConstantBuffer(mHandle); }
};

