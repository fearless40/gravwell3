
#pragma once

#include <hash_map>

#include "ComponentSystem.h"

namespace ComponentSys {

	template < class ID_External, class ID_Internal  >
	class LinkByID : public Linker<ID_External, ID_Internal> {
		std::hash_map< ID_ext, ID_in >  mapper;
	
	public:
		ID_in get(ID_ext id) {}
	
		ID_in link(ID_ext id, ID_in internal) {}
	
		// Returns the items Out_ID and then removes mapping
		ID_in unlink(ID_ext id) {}
		
		// Reverse look up. Can be much slower depending on the underlying data holder
		ID_in unlinkByInternal( ID_in internal );
	
		// Re-associates an 
		//void move(In_ID oldid, In_ID newid) {}
	
	};
};