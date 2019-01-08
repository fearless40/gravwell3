template <class DriverContext class DriverTraits>
class Context : DriverContext{
private:
	using T = DriverContext;
	using Traits = DriverTraits;
	constexpr auto base() { return static_cast<T*>(this); }

public:

	template< typename PipelineTraits > 
	struct PipelineMethods {
		using Traits = PipelineTraits;

		setConstantBuffer(Traits::ConstantBuffer & buf, uint32_t position) {
			Traits::setConstantBuffer(buf, position);
		}
		
		template< class Range >
		setConstantBuffers()(Range & ConstantBuffers, uint32_t startslot) {
			auto size = std::distance(begin(ConstantBuffers), end(ConstantBuffers));

		}

		setShader(Traits::Shader & shader);
		template <class Range>
		setShaders(Range & Shaders);


		setShaderResource(Traits::ShaderResource & resource);
		template <class Range>
		setShaderResources(Range & ShaderResources);

	};
	
	//Traits::IndexBuffer createIndexBuffer();
	//void writeIndexBuffer(Traits::IndexBuffer & buf, void * mem, std::size_t size);

	void writeConstantBuffer(Traits::ConstantBuffer & buf, void * mem, std::size_t size) {
		base()->writeConstantBuffer(buf, mem, size);
	}

	void setConstantBuffer(Traits::ConstantBuffer & buf, uint32_t position) {
		base()->
	}

	VertextShader vs;

};