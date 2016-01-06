#pragma once
#include <functional>
#include <unordered_map>
#include "../util/MinTypeSize.hpp"

namespace ComponentSys {
	
	/// Very simple class representing an opaque value
	// Template Parameter: Type is only used by the compiler to prevent ID from being 
	//		crossed into different classes. 

	template <class Type, int BitSize = 16> 
	struct ID {
		typedef typename Util::MinTypeSize<BitSize>::type Value_Type;
		typedef ID<Type, BitSize> type;
		Value_Type value;

		struct hash {
			std::size_t operator() (const type & k) const {
				return std::hash<type::Value_Type>()(k.value);
			}
		};


		template<class ValueType>
		class unordered_map : public std::unordered_map<type, ValueType, typename type::hash> {
			//typedef typename std::unordered_map<type, ValueType, typename type::hash> type;
		};

		bool operator == (const type & k) const {
			return value == k.value;
		}

		bool operator != (const type & k) const {
			return value != k.value;
		}

		bool operator < (const type & k) const {
			return value < k.value;
		}

		bool operator > (const type & k) const {
			return value > k.value;
		}

		bool operator <= (const type & k) const {
			return value <= k.value;
		}

		bool operator >= (const type & k) const {
			return value >= k.value;
		}

	
		
	};

	
	

	

}

