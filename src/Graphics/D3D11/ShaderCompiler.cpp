#include "../../stdafx.h"
#include "../Shared.h"
//#include <d3d11.h>
#include <D3DCompiler.h>

#include "ShaderCompiler.h"


namespace Graphics::D3D11::ShaderCompiler {

	CompiledShader compile(std::string_view shader) {
		ComPtr<ID3DBlob> CompiledShader = nullptr;
		ComPtr<ID3DBlob> error = nullptr;

		auto hr = D3DCompile(shader.data(), shader.size(), NULL, NULL, D3D_COMPILE_STANDARD_FILE_INCLUDE, "main", "vs_5_0", 0, 0, &CompiledShader, &error);

		if (FAILED(hr))
		{
			return { false,  error };
		}

		return { true, CompiledShader };
	}

	std::string_view CompiledShader::error_string() {
		if (hasError && shader.Get() != nullptr) {
			return { static_cast<char*>(shader->GetBufferPointer()), shader->GetBufferSize() };
		}
		return { nullptr };
	}
}