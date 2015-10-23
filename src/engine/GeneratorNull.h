#pragma once

#include "ComponentSystem.h"

namespace ComponentSys {
	
	// Null generator. 
	// Represents no way to generate an local handle to the item.
	
	struct GeneratorNull : GeneratorTraits<void, void> {
		// Can leave it empty as Component Will not call the functions ... 
	};
}