#pragma once

#include "Buffer.h"
#include "Buffer1D.h"
#include "Buffer2D.h"
//#include "IndexBuffer.h"

namespace Graphics::Generic {
	
	enum class VideoOutputs {
		Primary,
		Secondary
	};

	enum class VideoCards {
		Primary,
		Secondary
	};

	struct DisplayMode {
		unsigned int width = 0, height = 0;
		struct RefreshRate {
			unsigned int numerator = 0;
			unsigned int denominator = 0;
		} refreshrate;
		bool useVsync = false;
		bool useFullscreen = false;

	};



	enum class IndexBufferFormat {
			UINT_16,
			UINT_32
	};
	
	


	template <class DriverBase>
	class Driver : public DriverBase 
	{
	private:
		constexpr auto base() { return static_cast<DriverBase*>(this); }
	public:
		// Creation Functions
		template <typename T>
		auto createConstantBuffer(const Graphics::Generic::Buffer<T> & buf) {
			return base()->createConstantBuffer(buf.getMemory(), buf.getSize());
		}

		auto createConstantBuffer(void * data, std::size_t size) {
			return base()->createConstantBuffer(data, size);
		}

		template <typename T>
		auto createConstantBuffer(const std::vector<T> & vec) {
			return base()->createConstantBuffer(vec.data(), vec.size() * sizeof(T));
		}
		/*
		template <typename T>
		auto createVertexBuffer(const Graphics::Generic::Buffer<T> & buf, Graphics::Generic::BufferBinding binding) {
			return base()->createVertexBuffer(buf, binding);
		}

		template <typename T>
		auto createIndexBuffer(const Graphics::Generic::Buffer<T> & buf, Graphics::Generic::BufferBinding binding, IndexBuffer::Format fmt = IndexBuffer::Format::UINT_16) {
			auto ib = base()->createIndexBuffer(buf, binding);
			ib.mFormat = fmt;
			return ib;
		}

		template< typename VertexDescription >
		void registerVertexDescription(VertexDescription & vd) {
			base()->registerVertexDescription(vd);
		}
		*/
	};
	

}

