#pragma once

#include "ComponentSystem.h"

namespace ComponentSys {
	template <class DataType, ViewMemoryUse memtype, class Allocator = NullAllocator>
	class View : ViewTraits<memtype> {
		typedef DataType Data_Type;
		typedef Data_Type * Data_Iterator;

		Data_Iterator data;
		size_t size;

	public:
		Data_Iterator begin() { return data; }

		Data_Iterator end() { return data[size + 1]; }

	};
}