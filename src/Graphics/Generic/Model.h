namespace Graphics::Generic {
	
	template<typename VertexBuffer, typename IndexBuffer, typename VertexType> 
	struct Model {
		VertexBuffer mVertexBuffer;
		IndexBuffer  mIndexBuffer;
		VertexType   mVertexType;
		Topology mTopology;
		uint_32t mNbrIndicesToDraw{ 0 };
		uint_32t mStartIndexLocation{ 0 };
		int		 mBaseLocation{ 0 };
	};
}