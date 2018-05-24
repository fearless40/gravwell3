#pragma once
#include <string_view>
#include <gsl/span>

namespace Graphics::D3D11::ShaderCompiler {
	struct CompiledShader {
		CompiledShader(bool err, ComPtr<ID3DBlob> shader_data) :
			hasError(err), shader(shader_data) {}

		ComPtr<ID3DBlob> shader;

		operator bool() const { return hasError; }
		operator ID3DBlob * () const { return shader.Get(); }
		std::string_view error_string();
		auto operator -> () { return shader.Get(); }
		gsl::span<unsigned char> asSpan() { 
			return { (unsigned char *)(shader->GetBufferPointer()),
			(std::ptrdiff_t)shader->GetBufferSize() };
		};

	private:
		bool hasError;
	};

	CompiledShader compile(std::string_view shader);
}