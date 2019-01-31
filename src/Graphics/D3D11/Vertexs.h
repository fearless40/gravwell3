#pragma once
#include <string_view>
#include <vector>
#include <gsl/span>
#include "../Generic/Vertexs.h"

namespace Graphics::D3D11 {

	//template <typename T, std::size_t N>
	//constexpr auto ArraySize(T(&)[N]) { return N; }
	
	namespace detail {
		
		template< typename VertexType>
		struct VertexTypeInfo {
			using requires_compilation = std::false_type;
		};

		template<> 	struct VertexTypeInfo<Graphics::Generic::VertexTypes::Position> 
		{
			using requires_compilation = std::true_type;
			constexpr static char element_string[] = "float4 main( float4 position : Position ) : SV_Position { return position; }";
			constexpr static D3D11_INPUT_ELEMENT_DESC element_desc[] = {
				{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			};

			constexpr std::string_view getString() const { return element_string; }
			constexpr gsl::span<const D3D11_INPUT_ELEMENT_DESC> getDesc() const { return { element_desc }; }
		};

		template<> 	struct VertexTypeInfo<Graphics::Generic::VertexTypes::PositionColor>
		{
			using requires_compilation = std::true_type;
			constexpr static char element_string[] = "float4 main( float4 position : Position, float4 color : Color ) : SV_Position { return position; }";
			constexpr static D3D11_INPUT_ELEMENT_DESC element_desc[] = {
				{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
				{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }, 
			};

			constexpr std::string_view getString() const { return element_string; }
			constexpr gsl::span<const D3D11_INPUT_ELEMENT_DESC> getDesc() const { return { element_desc }; }
		};

		template<> 	struct VertexTypeInfo<Graphics::Generic::VertexTypes::PositionUV>
		{
			using requires_compilation = std::true_type;
			constexpr static char element_string[] = "float4 main( float4 position : Position, float2 uv : TexCoord ) : SV_Position { return position; }";
			constexpr static D3D11_INPUT_ELEMENT_DESC element_desc[] = {
				{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
				{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }, 
			};

			constexpr std::string_view getString() const { return element_string; }
			constexpr gsl::span<const D3D11_INPUT_ELEMENT_DESC> getDesc() const { return { element_desc }; }
		};

		template<> 	struct VertexTypeInfo<Graphics::Generic::VertexTypes::PositionNormalUV>
		{
			using requires_compilation = std::true_type;
			constexpr static char element_string[] =
R"(
struct VIn {
	float3 position : POSITION;
	float3 normal : NORMAL;
	float2 uv : TexCoord;
};

struct VOut {
	float4 position : SV_POSITION;
	float4 normal : TEXCOORD2;
	float2 uv : TEXCOORD0;
};

VOut main(VIn input) 
{ 
	VOut output;
	output.position = float4(input.position, 1.0f);
	output.normal = float4(input.normal, 1);
	output.uv = input.uv;
	return output;
}
)";
			constexpr static D3D11_INPUT_ELEMENT_DESC element_desc[] = {
				{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
				{ "NORMAL"	, 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
				{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT	, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			};

			constexpr std::string_view getString() const { return element_string; }
			constexpr gsl::span<const D3D11_INPUT_ELEMENT_DESC> getDesc() const { return { element_desc }; }
		};

	}

	template <typename VertexType>
	class VertexDescription 
	{
	private:
		static comptr<ID3D11InputLayout> mLayout;
		using vti = detail::VertexTypeInfo<VertexType>;

	public:
		
		using vertex_type = VertexType;

		// Set to false if you already compiled the layout string (std::false_type)
		using requires_compilation = typename vti::requires_compilation;

		static constexpr auto stride() { return sizeof(VertexType); }

		static ID3D11InputLayout * getInputLayout();
		ID3D11InputLayout * get() { return mLayout.get(); }
		//auto releaseAndGetAddressOf() { return mLayout.ReleaseAndGetAddressOf(); }
		auto operator & () { mLayout = nullptr;  return mLayout.put(); }
		operator ID3D11InputLayout * () { return get(); }
		
		// If you don't require compilation then you can ignore the next function in your class
		auto layoutstring() { return vti{}.getString(); }

		// If you want to return a compiled shader than you must write this function and return a
		// span<unsigned char> with the data within the span 
		auto compiledshader() { return gsl::span<unsigned char>{nullptr}; }

		// You must provide a layout for the runtime see the detail section above
		auto layoutarray() { return vti{}.getDesc(); }
	};

	template <typename VertexType>
	comptr<ID3D11InputLayout> VertexDescription<VertexType>::mLayout;

	template<typename T>
	ID3D11InputLayout * VertexDescription<T>::getInputLayout() {
		return VertexDescription<T>::mLayout.get();
	}
}