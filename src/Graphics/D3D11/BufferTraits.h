#pragma once

#include <d3d11.h>
#include "../Generic/BufferTraits.h"

namespace Graphics::D3D11::BufferTraits {

	namespace gen = Graphics::Generic::BufferTraits;
	namespace impl {
		template <typename BufferMapper>
		struct BufferTraits_Decoder {

		};

		template<>
		struct BufferTraits_Decoder < gen::Binding::Default >{
			static constexpr D3D11_USAGE value{ D3D11_USAGE_DEFAULT };
		};

		template <>
		struct  BufferTraits_Decoder <gen::Binding::Dynamic> {
			static constexpr D3D11_USAGE value{ D3D11_USAGE_DYNAMIC };
		};

		template <>
		struct  BufferTraits_Decoder <gen::Binding::Immutable> {
			static constexpr D3D11_USAGE value{ D3D11_USAGE_IMMUTABLE };
		};

		template <>
		struct  BufferTraits_Decoder <gen::Binding::Staging> {
			static constexpr D3D11_USAGE value{ D3D11_USAGE_STAGING };
		};

		template <>
		struct  BufferTraits_Decoder <gen::CPUAccess::Default> {
			static constexpr uint32_t value{ 0 };
		};

		template <>
		struct  BufferTraits_Decoder <gen::CPUAccess::Read> {
			static constexpr uint32_t value{ D3D11_CPU_ACCESS_READ };
		};

		template <>
		struct BufferTraits_Decoder  <gen::CPUAccess::Write> {
			static constexpr uint32_t value{ D3D11_CPU_ACCESS_WRITE };
		};

		template <>
		struct  BufferTraits_Decoder <gen::CPUAccess::ReadWrite> {
			static constexpr uint32_t value{ D3D11_CPU_ACCESS_WRITE | D3D11_CPU_ACCESS_READ };
		};
	}

	template <typename GenericBinding> 
	struct BufferTraits {
		static constexpr D3D11_USAGE Binding{ impl::BufferTraits_Decoder<GenericBinding::Binding>::value };
		static constexpr uint32_t CPU{ impl::BufferTraits_Decoder<GenericBinding::CPU>::value };
	};


}