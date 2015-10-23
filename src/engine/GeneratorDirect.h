#pragma once


#include "ComponentSystem.h"

namespace ComponentSys {
    
    // Simple Class that allows direct look up of a data item. 
    // The compiler will remove all the function call away making it as 
    // fast as direct array lookup (depending on how the DataStorage is setup to run)
    
    template <class index> 
    struct GeneratorDirect : GeneratorTraits<index,index> {
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