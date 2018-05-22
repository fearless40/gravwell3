#pragma once

#include "Handles.h"
#include "D3DPointer.h"
#include "../util/GenericHandleManager.h"
struct ID3D11VertexShader;

typedef D3DPointer<ID3D11VertexShader> VertexShader;

class VertexShaderManager
{
public:
	VertexShaderManager(void);
	~VertexShaderManager(void);

	static VertexShaderHandle newVertexShader( VertexShader & shader );
	
	DECLARE_MANAGER( VertexShader, VertexShaderHandle );

};

