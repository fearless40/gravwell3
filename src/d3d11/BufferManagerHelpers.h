
#pragma once

#include "D3DPointer.h"
class  D3D11;
#include <d3d11.h>

namespace BufferHelpers
{

	typedef D3DPointer<ID3D11Buffer> BPtr;

	enum BindingTypes
	{
		Immutable	= 0,
		Static		= 1,
		Dynamic		= 2,
		Partial		= 3
	};

	struct BufferItem 
	{
		D3DPointer<ID3D11Buffer> mData;
		uint32_t				 mTag;
	};

	ID3D11Buffer * createBuffer( D3D11 & d3d, void * mem, unsigned int memSize, D3D11_USAGE bufferMemoryType, unsigned int bindFlags );
	ID3D11Buffer * createConstantBuffer( D3D11 & d3d, void * data, unsigned int memSize );

	uint32_t getSizeOfBuffer( ID3D11Buffer * buf );

	void mapBuffer( D3D11 & d3d, ID3D11Buffer * buf, void * data, uint32_t memSize, D3D11_MAP mapType );

	//void editConstantBuffer( D3D11 * d3d, ID3D11Buffer * buf, void * data, uint32_t memSize );

};