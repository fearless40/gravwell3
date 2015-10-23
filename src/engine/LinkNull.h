#pragma once

#include "ComponentSystem.h"

namespace ComponentSys {
    
    // Null Linker. 
    // Will not allow external linkadge to the class
    
	struct LinkNull : LinkerTraits<void, void> {
		// Can leave it empty as Component Will not call the functions ... 
	};
}