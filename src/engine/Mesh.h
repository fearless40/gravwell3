#pragma once

#include "../Graphics/Generic/Vertexs.h"
#include "../Graphics/Generic/IndexSizes.h"

namespace Engine {

	struct Mesh {
		using vertex_type = Graphics::Generic::VertexTypes::PositionNormalUV;
		using index_definition = Graphics::Generic::IndexSize::Size16;
		using index_type = index_definition::type;
		
		std::vector<vertex_type> vertexs;
		std::vector<index_type> indicies;
	};

	struct MeshView {
		gsl::span<const std::byte> vertex_data;
		gsl::span<const std::byte> index_data;
	};
}