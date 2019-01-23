#pragma once

namespace Graphics::Generic {
	

	template<typename VertexBufferT, typename IndexBufferT, typename VertexTypeT, typename TopologyT, typename IndexSize> 
	struct Mesh {
		VertexBufferT	vertexBuffer;
		IndexBufferT	indexBuffer; 
		uint32_t		index_count{ 0 };
		uint32_t		index_start{ 0 };

		using vertex_type = VertexTypeT;
		using index_definition = IndexSize;
		using index_type = typename index_definition::type;
	};

	template<typename VertexBufferT, typename IndexBufferT, typename VertexTypeT, typename TopologyT>
	struct MeshAtlas {
		VertexBufferT	vertexBuffer;
		IndexBufferT	indexBuffer;

	};

}