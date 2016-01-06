#pragma once


#include "ComponentSystem.h"
#include "ID.h"
#include <unordered_map>

namespace ComponentSys {
	
	// Simple Class that allows direct look up of a data item. 
	// The compiler will remove all the function call away making it as 
	// fast as direct array lookup (depending on how the DataStorage is setup to run)
	
	template <class OutValue, class IDType = ID<GeneratorID<OutValue> >
	class GeneratorID : GeneratorTraits<IDType,OutValue> {
		uint32_t nextID = 0;
		

	public:
		ID_ext create(ID_in index) {
			return nextID++;
		}
		ID_in get(ID_ext index) {
			return index;
		}
		ID_in remove(ID_ext index) {
			return index;
		}
	};
}