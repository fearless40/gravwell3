#pragma once

#include "Buffer.h"

namespace Graphics::Generic {
	
	template<typename T>
	class Buffer1D : public Buffer<Buffer1D<T>> {
	private:
		std::vector<T> mData;

	public:
		using type = T;
		using vector_type = std::vector<T>;

		constexpr static std::size_t ElementSize = sizeof(T);

		Buffer1D() = default;
		Buffer1D(const Buffer1D &) = default;
		Buffer1D(Buffer1D &&) = default;
		Buffer1D(vector_type && vt) : mData(std::move(vt)) {}
		Buffer1D(const vector_type & vt) : mData(vt) {}
		
		Buffer1D & operator = (const Buffer1D &) = default;
		Buffer1D & operator = (Buffer1D &&) = default;

		template<typename Range>
		Buffer1D(const Range & r) : mData(std::begin(r), std::end(r)) {}

		auto operator = (vector_type && vt) {
			mData = std::move(vt);
			return *this;
		}

		auto operator = (const vector_type & vt) {
			mData.assign(std::begin(vt), std::end(vt));
			return *this;
		}



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
			return static_cast<void*>(mData.data());
		}
	};
}