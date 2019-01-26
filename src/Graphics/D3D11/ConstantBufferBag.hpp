#pragma once

#include "../../util/TupleHelper.h"

namespace Graphics::D3D11 {

	template <typename ... BufferTypes>
	class ConstantBufferBag {
		std::array< comptr<ID3D11Buffer>, sizeof...(BufferTypes)> mBuffers;

		template<typename BufferType>
		constexpr int get_cb_index() const {
			using types_t = std::tuple<BufferTypes...>;
			return util::Index<BufferType, types_t>::value;
		}

	public:

		template<typename BufferType>
		ID3D11Buffer * get() const noexcept {
			return mBuffers[get_cb_index<BufferType>()].get();
		}

		ID3D11Buffer * get(std::size_t index) const noexcept{
			return mBuffers[index];
		}

		template<typename BufferType>
		comptr<ID3D11Buffer> & get_comptr() const noexcept {
			return mBuffers[get_cb_index<BufferType>()];
		}

		comptr<ID3D11Buffer> & get_comptr(std::size_t index) const noexcept {
			return mBuffers[index];
		}

		constexpr auto begin() { return mBuffers.begin(); }
		constexpr auto end() { return mBuffers.end(); }

		void clear() {
			for (auto & item : mBuffers)
				item = nullptr;
		}

		constexpr std::size_t size() { return mBuffers.size(); }
	};
}