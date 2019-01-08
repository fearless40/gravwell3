enum class IndexBufferFormat {
	UINT_16,
	UINT_32
};

template <typename DriverType>
struct IndexBuffer : DriverType {
	IndexBufferFormat format;
};

