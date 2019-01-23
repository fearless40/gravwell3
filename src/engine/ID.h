#pragma once
#include <cstdint>

namespace Engine {
	
	/// Very simple class representing an opaque value
	// Template Parameter: Type is only used by the compiler to prevent ID from being 
	//		crossed into different classes. 

	namespace details {
		template<unsigned int Sz>
		struct GetSizeType {
		};

		template<>
		struct GetSizeType<2> {
			using type = uint16_t;
		};

		template<>
		struct GetSizeType<4> {
			using type = uint32_t;
		};

		template<>
		struct GetSizeType<8> {
			using type = uint64_t;
		};

		template<>
		struct GetSizeType<1> {
			using type = uint8_t;
		};

	}

	template <class TypeTag, typename UnderLyingType> 
	struct ID {
		using value_type = UnderLyingType;
		
		UnderLyingType value;

		struct hash {
			std::size_t operator() (const value_type & k) const {
				return std::hash<value_type>()(k.value);
			}
		};

		value_type get() const { return value; }

		bool operator == (const ID & k) const {
			return value == k.value;
		}

		bool operator != (const ID & k) const {
			return value != k.value;
		}

		bool operator < (const ID & k) const {
			return value < k.value;
		}

		bool operator > (const ID & k) const {
			return value > k.value;
		}

		bool operator <= (const ID & k) const {
			return value <= k.value;
		}

		bool operator >= (const ID & k) const {
			return value >= k.value;
		}
	};
}

