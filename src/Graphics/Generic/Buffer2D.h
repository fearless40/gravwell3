#pragma once

namespace Graphics::Generic {
	
	template<typename T>
	class Buffer2D : public Buffer<Buffer2D<T>> {
	private:
			std::vector<T> mData;
			std::size_t mWidth;
			std::size_t mHeight;

			constexpr std::size_t index(std::size_t x, std::size_t y) {
				return (x * ElementSize) + (mWidth * y);
			}

	public:
		Buffer2D(std::size_t width, std::size_t height) : mWidth(width), mHeight(height) {
			reserve(index(width, height));
		}

		using type = T;
		constexpr static std::size_t ElementSize = sizeof(T);
		
		auto begin() { return mData.begin(); }
		auto end() { return mData.end(); }
		auto begin()	const noexcept { return mData.cbegin(); }
		auto end()		const noexcept { return mData.cend(); }

		void push_back(const T & t) { mData.push_back(t); }
		void push_back(T && t) { mData.push_back(std::forward(t)); }

		void reserve(std::size_t sz) { mData.reserve(sz); }

		T & at(std::size_t x, std::size_t y) {
			return mData.at(index(x, y));
		}

		T & operator ()(std::size_t x, std::size_t y) {
			return mData[index(x, y)];
		}

		auto IteratorAt(std::size_t x, std::size_t y) {
			return mData.begin() + index(x, y);
		}

		auto IteratorAt (std::size_t x, std::size_t y) const {
			return mData.cbegin() + index(x, y);
		}


		std::size_t  getSize(BufferSizes bs) const noexcept {
			switch (bs) {
			case BufferSizes::Linear:
				// Return nbr of bytes per line
				return mWidth * ElementSize;

			case BufferSizes::Texture2D:
				// Return total texture size
				assert(mData.size() * ElementSize == mWidth * mHeight * ElementSize);
				return mData.size() * ElementSize;
			default:
				return 0;
			}
		}

		const void * getMemory() const noexcept {
			return static_cast<void*>mData.data();
		}
	};

}