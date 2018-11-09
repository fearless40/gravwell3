#pragma once

#include "Buffer.h"
#include "Buffer1D.h"
#include "Buffer2D.h"
#include "IndexBuffer.h"

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

	enum class BufferBinding
	{
		Default,
		Immutable,
		Dynamic,
		Staging
	};

	enum class IndexBufferFormat {
			UINT_16,
			UINT_32
	};
	
	enum class Topology {
		PointList,
		LineList,
		LineStrip,
		TriangleList,
		TriangleStrip
	};

	/*template <class Base>
	class Driver : public Base 
	{
	private:
		constexpr auto base() { return static_cast<Base*>(this); }
	public:
		template<typename DriverSpecific>
		static std::unique_ptr<Driver> CreateDevice(DriverSpecific hwnd,
			const Graphics::Generic::DisplayMode mode)
		{
			return Base::CreateDevice(hwnd, mode);
		}

		void setupDefaults() {
			base()->setupDefaults();
		}

		void resize(unsigned int width, unsigned int height) {
			base()->resize(width,height);
		}
		
		void present() {
			base()->present();
		}

		// Creation Functions
		template <typename T>
		auto createConstantBuffer(const Graphics::Generic::Buffer<T> & buf) {
			return base()->createConstantBuffer(buf);
		}

		template <typename T>
		auto createVertexBuffer(const Graphics::Generic::Buffer<T> & buf, Graphics::Generic::BufferBinding binding) {
			return base()->createVertexBuffer(buf, binding);
		}

		template <typename T>
		auto createIndexBuffer(const Graphics::Generic::Buffer<T> & buf, Graphics::Generic::BufferBinding binding, IndexBuffer::Format fmt = IndexBuffer::Format::UINT_16) {
			return IndexBuffer{ base()->createIndexBuffer(buf, binding), fmt };
		}

		template< typename VertexDescription >
		void registerVertexDescription(VertexDescription & vd) {
			base()->registerVertexDescription(vd);
		}
	};
	*/

}

