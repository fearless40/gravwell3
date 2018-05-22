#include "../stdafx.h"
#include <d3d11.h>
#include "VertexShaderManager.h"


VertexShaderManager::VertexShaderManager(void)
{
}


VertexShaderManager::~VertexShaderManager(void)
{
}


IMPLEMENT_MANAGER( VertexShader, VertexShaderHandle, VertexShaderManager );


VertexShaderHandle VertexShaderManager::newVertexShader( VertexShader & shader )
{
	VertexShaderHandle vsh = newHandle();
	fromHandle( vsh ) = shader;
	return vsh;
}