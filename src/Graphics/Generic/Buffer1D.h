#pragma once

#include "Buffer.h"

namespace Graphics::Generic {
	
	template<typename T>
	class Buffer1D : public Buffer<Buffer1D<T>> {
	private:
		std::vector<T> mData;

	public:
		using type = T;
		constexpr static std::size_t ElementSize = sizeof(T);

		auto begin()					{ return mData.begin(); }
		auto end()						{ return mData.end(); }
		auto begin()	const noexcept	{ return mData.cbegin(); }
		auto end()		const noexcept	{ return mData.cend(); }

		void push_back(const T & t) { mData.push_back(t); }
		void push_back(T && t) { mData.push_back(std::forward(t)); }

		void reserve(std::size_t sz) { mData.reserve(sz); }
		
		std::size_t  getSize(BufferSizes bs) const noexcept {
			switch (bs) {
			case BufferSizes::Linear:
				return mData.size() * ElementSize;
			default:
				return 0;
			}
		}

		const void * getMemory() const noexcept{
			return static_cast<void*>mData.data();
		}
	};
}