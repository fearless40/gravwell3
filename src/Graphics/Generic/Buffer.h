#pragma once

namespace Graphics::Generic {

	enum class BufferSizes {
		Linear,
		Texture2D,
		Texture3D
	};

	template< typename T >
	class Buffer 
	{
	public:
		std::size_t  getSize(BufferSizes bs = BufferSizes::Linear) {
			return static_cast<T*>(this)->getSize(bs);
		}
		const void * getMemory() {
			return static_cast<T*>(this)->getMemory();
		}
		
	};
}