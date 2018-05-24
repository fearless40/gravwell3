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
			constexpr static char * element_string = "float4 main( float4 position : Position ) : SV_Position { return position; }";
			constexpr static D3D11_INPUT_ELEMENT_DESC element_desc[] = {
				{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			};

			constexpr std::string_view getString() const { return element_string; }
			constexpr gsl::span<const D3D11_INPUT_ELEMENT_DESC> getDesc() const { return { element_desc }; }
		};

		template<> 	struct VertexTypeInfo<Graphics::Generic::VertexTypes::PositionColor>
		{
			using requires_compilation = std::true_type;
			constexpr static char * element_string = "float4 main( float4 position : Position, float4 color : Color ) : SV_Position { return position; }";
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
			constexpr static char * element_string = "float4 main( float4 position : Position, float2 uv : TexCoord ) : SV_Position { return position; }";
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
			constexpr static char * element_string = "float4 main( float4 position : Position, float4 normal : Normal, float2 uv : TexCoord ) : SV_Position { return position; }";
			constexpr static D3D11_INPUT_ELEMENT_DESC element_desc[] = {
				{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
				{ "NORMAL", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
				{ "TEXCOORD",  0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			};

			constexpr std::string_view getString() const { return element_string; }
			constexpr gsl::span<const D3D11_INPUT_ELEMENT_DESC> getDesc() const { return { element_desc }; }
		};

	}

	template <typename VertexType>
	class VertexDescription : public Graphics::Generic::VertexDescription<VertexDescription<VertexType>, VertexType>
	{
	private:
		static ComPtr<ID3D11InputLayout> mLayout;
		using vti = detail::VertexTypeInfo<VertexType>;

	public:
		
		using vertex_type = typename Graphics::Generic::VertexDescription<VertexDescription, VertexType>::vertex_type;
		using base_type = typename  Graphics::Generic::VertexDescription<VertexDescription, VertexType>::base_type;
		
		// Set to false if you already compiled the layout string (std::false_type)
		using requires_compilation = typename vti::requires_compilation;

		ID3D11InputLayout * get() { return mLayout.Get(); }
		auto releaseAndGetAddressOf() { return mLayout.ReleaseAndGetAddressOf(); }
		auto operator & () { return mLayout.ReleaseAndGetAddressOf(); }
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
	ComPtr<ID3D11InputLayout> VertexDescription<VertexType>::mLayout;



	
}