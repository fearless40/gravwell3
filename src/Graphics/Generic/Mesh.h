namespace Graphics::Generic {
	

	template<typename VertexBufferT, typename IndexBufferT, typename VertexTypeT, typename TopologyT, typename IndexSize> 
	struct Mesh {
		VertexBufferT	vertexBuffer;
		IndexBufferT	indexBuffer; 
		uint32_t		index_count{ 0 };
		uint32_t		index_start{ 0 };
	};

	template<typename VertexBufferT, typename IndexBufferT, typename VertexTypeT, typename TopologyT>
	struct MeshAtlas {
		VertexBufferT	vertexBuffer;
		IndexBufferT	indexBuffer;

	};

}