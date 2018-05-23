#include "../../stdafx.h"
#include "../Shared.h"
#include <d3d11.h>
#include <D3DCompiler.h>
#include "Vertexs.h"
#include "Driver.h"

namespace Graphics::D3D11
{
	ComPtr<ID3D11InputLayout> Vertexs::addCustomLayout(Driver * dr, std::string_view validation,
		D3D11_INPUT_ELEMENT_DESC * inputlayout, unsigned int nbrInputLayout) {
		ComPtr<ID3DBlob> CompiledShader = nullptr;
		ComPtr<ID3DBlob> error = nullptr;
		ComPtr<ID3D11InputLayout> layout;

		auto hr = D3DCompile(validation.data(), validation.size(), NULL, NULL, D3D_COMPILE_STANDARD_FILE_INCLUDE, "main", "vs_5_0", 0, 0, &CompiledShader, &error);

		if (FAILED(hr))
		{
			char * buffer = (char *)error->GetBufferPointer();
			int size = error->GetBufferSize();
			return;
		}

		hr = dr->get()->CreateInputLayout(inputlayout, nbrInputLayout, CompiledShader->GetBufferPointer(), CompiledShader->GetBufferSize(), &layout);

		if (FAILED(hr))
		{
			return;
		}
	}
}