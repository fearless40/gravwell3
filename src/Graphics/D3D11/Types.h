#pragma once

#include <type_traits>
#include "../Generic/Mesh.h"
#include "../Generic/Vertexs.h"
#include "../Generic/Topology.h"
#include "../Generic/IndexSizes.h"

namespace Graphics::D3D11 {
	using MeshNUV = Graphics::Generic::Mesh<comptr<ID3D11Resource>,
		comptr<ID3D11Resource>,
		Graphics::Generic::VertexTypes::PositionNormalUV,
		Graphics::Generic::Topology::TriangleStrip,
		Graphics::Generic::IndexSize::Size16>;

	template<typename T>
	constexpr D3D11_PRIMITIVE_TOPOLOGY Topology() {
		namespace tp = Graphics::Generic::Topology;
		if constexpr (std::is_same_v<T::topology_type, tp::TriangleStrip>) {
			return D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP;
		}
		else if constexpr (std::is_same_v<T::topology_type, tp::TriangleList>) {
			return D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
		}
		else {
			return D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP;
		}
	}

	template <typename IndexType> 
	constexpr DXGI_FORMAT IndexSize() {
		namespace is = Graphics::Generic::IndexSize;
		if constexpr (std::is_same_v<is::Size16, IndexType>) {
			return DXGI_FORMAT_R16_UINT;
		}
		else {
			return DXGI_FORMAT_R32_UINT;
		}
	}
}