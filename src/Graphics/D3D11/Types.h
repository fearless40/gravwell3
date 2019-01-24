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
		Graphics::Generic::Toplogy::TriangleStrip,
		Graphics::Generic::IndexSize::Size16>;

	template<typename T>
	constexpr D3D11_PRIMITIVE_TOPOLOGY Topology(T const & ) {
		using namespace tp = Graphics::Generic::Topology;
		if constexpr (std::is_same_v<T::TopologyT, tp::TriangleStrip>) {
			return D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP;
		}
	}

	template <typename MeshTypeT> 
	constexpr DXGI_FORMAT IndexSize() {
		using namespace is = Graphics::Generic::IndexSize;
		if constexpr (std::is_same_v<is::Size16, MeshTypeT::IndexSize>) {
			return DXGI_FORMAT_R16_UINT;
		}
		else constexpr {
			return DXGI_FORMAT_R32_UINT;
		}
	}
}