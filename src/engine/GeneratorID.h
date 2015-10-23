#pragma once


#include "ComponentSystem.h"
#include "ID.h"

namespace ComponentSys {
    
    // Simple Class that allows direct look up of a data item. 
    // The compiler will remove all the function call away making it as 
    // fast as direct array lookup (depending on how the DataStorage is setup to run)
    
    template <class OutValue> 
    struct GeneratorID : GeneratorTraits<ID<GeneratorID<OutValue> >,OutValue> {
        ID_ext create(ID_in index) {
			return index;
		}
		ID_in get(ID_ext index) {
			return index;
		}
		ID_in remove(ID_ext index) {
			return index;
		}
    };
}