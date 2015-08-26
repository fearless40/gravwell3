#include "../stdafx.h"
#include "D3D11.h"
#include <D3DCompiler.h>
#include "ShaderManager.h"


ShaderManager::ShaderManager(void)
{
}


ShaderManager::~ShaderManager(void)
{
}

ID3D11VertexShader * ShaderManager::loadVertexFromFile(D3D11 * d3d, std::wstring filename ) 
{
	ID3DBlob * blob;
	HRESULT hr;
	
	hr = D3DCompileFromFile( getRelativePath( filename, L"VertexShaders" ).c_str(), NULL, D3D_COMPILE_STANDARD_FILE_INCLUDE, "main", "vs_5_0", 0,0, &blob, NULL );

	if( FAILED(hr) )
	{
		return nullptr;
	}

	ID3D11VertexShader * vs = nullptr;

	hr = d3d->getDevice()->CreateVertexShader( blob->GetBufferPointer(), blob->GetBufferSize(), NULL, &vs );

	if( FAILED(hr) )
	{
		return nullptr;
	}

	return vs;

}

ID3D11PixelShader * ShaderManager::loadPixelFromFile(D3D11 * d3d, std::wstring filename )
{
	ID3DBlob * blob;
	HRESULT hr;
	
	hr = D3DCompileFromFile( getRelativePath( filename, L"PixelShaders" ).c_str(), NULL, D3D_COMPILE_STANDARD_FILE_INCLUDE, "main", "ps_5_0", 0,0, &blob, NULL );

	if( FAILED(hr) )
	{
		return nullptr;
	}

	ID3D11PixelShader * ps = nullptr;

	hr = d3d->getDevice()->CreatePixelShader( blob->GetBufferPointer(), blob->GetBufferSize(), NULL, &ps );

	if( FAILED(hr) )
	{
		return nullptr;
	}

	return ps;
}


std::wstring ShaderManager::getRelativePath(const std::wstring inputFileName, const std::wstring resourceFolder)
{
	wchar_t workingDir[256];
	
	GetCurrentDirectoryW(256, workingDir );

	std::wstring ret = workingDir;

	ret.append( L"\\Media\\");
	ret += resourceFolder;
	ret.append(L"\\");
	ret += inputFileName;
	ret.append(L".hlsl");

	return ret;
}

