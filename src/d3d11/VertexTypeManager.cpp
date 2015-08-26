#include "../stdafx.h"
#include "VertexTypeManager.h"
#include <d3d11.h>
#include "D3D11.h"
#include <D3DCompiler.h>





VertexTypeManager VertexTypeManager::_me;
VertexTypeManager::VertexTypeArray VertexTypeManager::mTypes;

VertexTypeManager::VertexTypeManager(void)
{
	mTypes.reserve(nbrVertexTypes);
}


VertexTypeManager::~VertexTypeManager(void)
{
}

void CompileShaderLayout(D3D11 * d3d, const char * shaderText, int sizeOfText, D3D11_INPUT_ELEMENT_DESC * arrayItems, int nbrItems, VertexTypeDescription & vtd )
{
	ID3DBlob * CompiledShader = nullptr;
	ID3DBlob * error;

	HRESULT hr;

	hr = D3DCompile( shaderText, sizeOfText, NULL, NULL, D3D_COMPILE_STANDARD_FILE_INCLUDE, "main", "vs_5_0", 0, 0, &CompiledShader, &error );

	if( FAILED(hr) )
	{
		char * buffer = (char *)error->GetBufferPointer();
		int size = error->GetBufferSize();


		return;
	}

	hr = d3d->getDevice()->CreateInputLayout( arrayItems, nbrItems, CompiledShader->GetBufferPointer(), CompiledShader->GetBufferSize(), &vtd.layout );

	if( FAILED(hr) )
	{
		return;
	}

	if( CompiledShader) 
		CompiledShader->Release();
	
	if( error )
		error->Release();

}

void VertexTypeManager::initalize(D3D11 * d3d)
{
	VertexTypeDescription vtd;
	
	// The order the vertex formats get added is very important. DO NOT FUCK WITH THE ORDER.
	// Please follow the Enum described in the header file. If adding new formats
	// follow the FUCKING enum values. 
	// Thanks for your kind understanding.

	// VertexPos = 0
	{
		const char VertexShader[] = "float4 main( float4 position : Position ) : SV_Position { return position; }";

		D3D11_INPUT_ELEMENT_DESC vtPos[] = {
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};

		vtd.stride = sizeof(VertexPos);

		CompileShaderLayout( d3d, VertexShader, sizeof(VertexShader), vtPos, 1, vtd );
	
		mTypes.push_back(vtd);
	}

	// VertexPosColor = 1
	{
		const char VertexShader[] = "float4 main( float4 position : Position, float4 color : Color ) : SV_Position { return position; }";

		D3D11_INPUT_ELEMENT_DESC vtPos[] = {
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};

		vtd.stride = sizeof(VertexPosColor);

		CompileShaderLayout( d3d, VertexShader, sizeof(VertexShader), vtPos, 2, vtd );
	
		mTypes.push_back(vtd);
	}

	// VertexPosUV = 2
	{
		const char VertexShader[] = "float4 main( float4 position : Position, float2 uv : TexCoord ) : SV_Position { return position; }";

		D3D11_INPUT_ELEMENT_DESC vtPos[] = {
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};

		vtd.stride = sizeof(VertexPosUV);

		CompileShaderLayout( d3d, VertexShader, sizeof(VertexShader), vtPos, 2, vtd );
	
		mTypes.push_back(vtd);
	}

	// VertexPosNormalUV = 3
	{
		const char VertexShader[] = "float4 main( float4 position : Position, float4 normal : Normal, float2 uv : TexCoord ) : SV_Position { return position; }";

		D3D11_INPUT_ELEMENT_DESC vtPos[] = {
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "NORMAL", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD",  0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};

		vtd.stride = sizeof(VertexPosNormalUV);

		CompileShaderLayout( d3d, VertexShader, sizeof(VertexShader), vtPos, 3, vtd );
	
		mTypes.push_back(vtd);
	}

}

void VertexTypeManager::shutdown()
{
	for( VertexTypeArray::iterator i = mTypes.begin(); i != mTypes.end(); ++i )
	{
		if(i->layout)
			i->layout->Release();
	}
}

VertexTypeDescription VertexTypeManager::getVertexType( VertexTypes vt )
{
	return mTypes[vt];
}