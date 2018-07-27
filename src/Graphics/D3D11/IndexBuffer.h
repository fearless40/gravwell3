#pragma once

namespace Graphics::Generic {

	

	template<typename InternalBuffer>
	class IndexBuffer {
		InternalBuffer mBuf;
		DXGI_FORMAT mFmt = DXGI_FORMAT_R16_UINT;
	
	public:
		IndexBuffer(InternalBuffer && buf) : mBuf(std::forward(buf)) {	}

		IndexBuffer(InternalBuffer && buf, IndexBuffer::Format fmt) : mBuf(std::forward(buf)), mFmt(fmt) {}

		InternalBuffer & buffer() { return mBuf; }
		DXGI_FORMAT format() { return mFmt; }
	};
}