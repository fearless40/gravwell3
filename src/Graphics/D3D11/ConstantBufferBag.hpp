#pragma once

#include "../../util/TupleHelper.h"

namespace Graphics::D3D11 {

//	struct ID3D11Buffer;

	template <typename ... BufferTypes>
	class ConstantBufferBag {
		using ptr = comptr<ID3D11Buffer>;

		std::array< comptr<ID3D11Buffer>, sizeof...(BufferTypes)> mBuffers;

		template<typename BufferType>
		constexpr int get_cb_index() const {
			using types_t = std::tuple<BufferTypes...>;
			return util::Index<BufferType, types_t>::value;
		}

	public:

		using size_n = std::integral_constant<int, sizeof...(BufferTypes)>;

		template<typename BufferType>
		ID3D11Buffer * get() const noexcept {
			return get(get_cb_index<BufferType>());
		}

		ID3D11Buffer * get(std::size_t index) const noexcept{
			return mBuffers[index].get();
		}

		template<typename BufferType>
		comptr<ID3D11Buffer> & get_comptr() noexcept {
			return get_comptr(get_cb_index<BufferType>());
		}

		comptr<ID3D11Buffer> & get_comptr(std::size_t index) noexcept {
			return mBuffers[index];
		}

		template<typename BufferType>
		void set_comptr(comptr<ID3D11Buffer> & value) {
			set_comptr(get_cb_index<BufferType>(), std::forward<ptr>(value));
		}

		void set_comptr(std::size_t index, comptr <ID3D11Buffer> & value) {
			mBuffers[index] = nullptr;
			mBuffers[index].copy_from(value);
		}

		constexpr auto begin() { return mBuffers.begin(); }
		constexpr auto end() { return mBuffers.end(); }

		void clear() {
			for (auto & item : mBuffers)
				item = nullptr;
		}

		constexpr std::size_t size() const noexcept { return mBuffers.size(); }
	};
}