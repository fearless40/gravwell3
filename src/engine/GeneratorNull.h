#pragma once

#include "ComponentSystem.h"

namespace ComponentSys {
	struct GeneratorNull : GeneratorTraits<void, void> {
		// Can leave it empty as Component Will not call the functions ... 
	};
}