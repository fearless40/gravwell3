#pragma once

#include "ComponentSystem.h"
#include "Handle.h"

namespace ComponentSys {

	// Generator useful for generating handles.
	// Handles are a direct way to look up the data however the handle itself is an opaque value.
	// Useful as it only has one layer of indirection away from the actual DataStorage (depengin on how DataStorage is setup)

	template <int GenerationSize, int IndexSize>
	class GeneratorHandle : public GeneratorTraits< Handle<GenerationSize,IndexSize>::Index_Type, 
		Handle<GenerationSize, IndexSize>::type > {

	protected:
		ID_ext::Generation_Type * generations;
		uint32_t size;

	public:
		

		ID_ext create(ID_in value) {
			++generations[value];
			return ID_ext(generations[value], value);
		}
		ID_in get(ID_ext handle) {
			if (generations[handle.index()] == handle.generations() )
				return handle.index();
		}
		ID_in remove(ID_ext handle) {
			++generations[handle.index()];
			return handle.index();
		}
	};
};