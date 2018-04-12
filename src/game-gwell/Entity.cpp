#include "../stdafx.h"
#include "Entity.h"

namespace Entity {
	EntityID mNextID;
	
	struct EntityToString {
		EntityID id;
		std::size_t stringid;
	};

	std::vector<EntityToString> mEntityStringMap;
	std::vector<std::string> mStrings;

	EntityID create(Hint ht) {
		return ++mNextID;
	}

	void destroy(EntityID id) {
		//Do Nothing
	}

	bool isValidEntity(EntityID id) {
		return true;
	}
}