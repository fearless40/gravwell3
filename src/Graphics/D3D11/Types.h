#pragma once

#include "../Generic/Mesh.h"
#include "../Generic/Vertexs.h"
#include "../Generic/Topology.h"

namespace Graphics::D3D11 {
	using MeshNUV = Graphics::Generic::Mesh<comptr<ID3D11Resource>,
		comptr<ID3D11Resource>,
		Graphics::Generic::VertexTypes::PositionNormalUV,
		Graphics::Generic::Toplogy::TriangleStrip>;

	template<typename T>
	constexpr D3D11_PRIMITIVE_TOPOLOGY Topology(T const & t) {
		using namespace tp = Graphics::Generic::Topology;
		if constexpr (std::is_same_v<T::TopologyT, tp::TriangleStrip>) {
			return D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP;
		}
	}
}