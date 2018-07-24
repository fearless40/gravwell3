#pragma once

namespace Graphics::Generic {

	namespace IndexBuffer {
		enum class Format {
			UINT_16,
			UINT_32
		};
	}

	template<typename InternalBuffer>
	class IndexBuffer {
		InternalBuffer mBuf;
		IndexBuffer::Format mFmt = IndexBuffer::Format::UINT_16;
	
	public:
		IndexBuffer(InternalBuffer && buf) : mBuf(std::forward(buf)) {	}

		IndexBuffer(InternalBuffer && buf, IndexBuffer::Format fmt) : mBuf(std::forward(buf)), mFmt(fmt) {}

		InternalBuffer & buffer() { return mBuf; }
		IndexBuffer::Format format() { return mFmt; }
	};
}