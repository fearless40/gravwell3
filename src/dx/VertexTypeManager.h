#pragma once

#include <vector>
#include "VertexTypeDescription.h"
#include <DirectXMath.h>

class D3D11;

// Simple class to manager the different vertex types
// useful for sorting vertex types and also maintains 
// D3D verte data type definitions.

class VertexTypeManager
{
protected:
	typedef std::vector<VertexTypeDescription> VertexTypeArray;

	static VertexTypeArray mTypes;

	static VertexTypeManager _me;

public:
	VertexTypeManager(void);
	~VertexTypeManager(void);

	enum VertexTypes 
	{
		Pos				= 0,
		Pos_Color		= 1,
		Pos_UV			= 2,
		Pos_Normal_UV	= 3,
	};

	struct VertexPos { 
		DirectX::XMFLOAT4 position;
	};

	struct VertexPosColor {
		DirectX::XMFLOAT4 position;
		DirectX::XMFLOAT4 color;
	};

	struct VertexPosUV {
		DirectX::XMFLOAT4 position;
		DirectX::XMFLOAT2 uv;
	};

	struct VertexPosNormalUV {
		DirectX::XMFLOAT4 position;
		DirectX::XMFLOAT4 normal;
		DirectX::XMFLOAT2 uv;
	};

	static const int nbrVertexTypes = 4;

	static void initalize(D3D11 * d3d);
	static void shutdown();
		
	static VertexTypeDescription getVertexType( VertexTypes vt );
		

	//bool registerVertexType( std::string name, D3D11_INPUT_ELEMENT_DESC * items
};

