#pragma once 
#include "ComponentSystem.h"

namespace ComponentSys {
	
	template <class DataType, ViewMemoryUse memtype, class Allocator = NullAllocator>
	struct View {
		typedef DataType Data_Type;
		typedef Data_Type * Data_Iterator;
		
		Data_Iterator data;

		Data_Iterator begin() {}

		Data_Iterator end() {}

		//If random access allow an operator []
		Data_Iterator operator [] (IndexType t) {}
		
	};

