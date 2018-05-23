#pragma once
#include <string_view>
#include <vector>
#include "../Generic/Vertexs.h"

namespace Graphics::D3D11 {

	class Driver;
	class Vertexs {
	private:
		std::vector< ComPtr<ID3D11InputLayout> > mLayouts;

		template <typename T, std::size_t N>
		constexpr ArraySize( T (&)[N]) { return N; }

	public:
		ComPtr<ID3D11InputLayout> addCustomLayout(Driver * dr, std::string_view validation,
			D3D11_INPUT_ELEMENT_DESC * inputlayout, unsigned int nbrInputLayout);

		void intialize(Driver * dr) {
			mLayouts.clear();
			mLayouts.reserve(4);
			mLayouts.emplace_back(add<Graphics::Generic::Vertex::Position>(dr)); //0
			mLayouts.emplace_back(add<Graphics::Generic::Vertex::PositionColor>(dr)); //1
			mLayouts.emplace_back(add<Graphics::Generic::Vertex::PositionUV>(dr)); //2
			mLayouts.emplace_back(add<Graphics::Generic::Vertex::PositionNormalUV>(dr)); //3
		}

		template<typename VectorStruct>
		ComPtr<ID3D11InputLayout> add(Driver *) {
			static_assert(false, "Does not support adding custom types though this interface.")
		}

		template <> ComPtr<ID3D11InputLayout> add<Graphics::Generic::Vertex::Position>(Driver * dr) {
			const char VertexShader[] = "float4 main( float4 position : Position ) : SV_Position { return position; }";
			D3D11_INPUT_ELEMENT_DESC vtPos[] = {
				{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			};
			return addCustomLayout(dr, VertexShader, vtPos, ArraySize(vtPos));
		}

		template <> ComPtr<ID3D11InputLayout> add<Graphics::Generic::Vertex::PositionColor>(Driver * dr){
			const char VertexShader[] = "float4 main( float4 position : Position, float4 color : Color ) : SV_Position { return position; }";

			D3D11_INPUT_ELEMENT_DESC vtPos[] = {
				{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
				{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			};
			return addCustomLayout(dr, VertexShader, vtPos, ArraySize(vtPos));
		}

		template <> ComPtr<ID3D11InputLayout> add<Graphics::Generic::Vertex::PositionNormalUV>(Driver * dr) {
			const char VertexShader[] = "float4 main( float4 position : Position, float2 uv : TexCoord ) : SV_Position { return position; }";

			D3D11_INPUT_ELEMENT_DESC vtPos[] = {
				{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			};
			return addCustomLayout(dr, VertexShader, vtPos, ArraySize(vtPos));
		}

		template <> ComPtr<ID3D11InputLayout> add<Graphics::Generic::Vertex::PositionUV>(Driver * dr) {
			const char VertexShader[] = "float4 main( float4 position : Position, float4 normal : Normal, float2 uv : TexCoord ) : SV_Position { return position; }";

			D3D11_INPUT_ELEMENT_DESC vtPos[] = {
				{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "NORMAL", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD",  0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			};
			return addCustomLayout(dr, VertexShader, vtPos, ArraySize(vtPos));
		}

		template< typename VectorStruct>
		ID3D11InputLayout * get() { return nullptr; }

		template<> 	ID3D11InputLayout * get<Graphics::Generic::Vertex::Position>() {
			return mLayouts[0].Get(); 
		}

		template<> 	ID3D11InputLayout * get<Graphics::Generic::Vertex::PositionColor>() {
			return mLayouts[1].Get();
		}

		template<> 	ID3D11InputLayout * get<Graphics::Generic::Vertex::PositionUV>() {
			return mLayouts[2].Get();
		}
		
		template<> 	ID3D11InputLayout * get<Graphics::Generic::Vertex::PositionNormalUV>() {
			return mLayouts[3].Get();
		}

	};
}