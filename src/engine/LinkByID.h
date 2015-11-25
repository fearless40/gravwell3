
#pragma once

#include <unordered_map>

#include "ComponentSystem.h"

namespace ComponentSys {

	template < class ID_External, class ID_Internal  >
	class LinkByID : public Linker<ID_External, ID_Internal> {
		
		std::unordered_map<ID_External, ID_Internal>  etoi;
		std::unordered_map<ID_Internal, ID_External>  itoe;

	public:
		ID_in get(ID_ext id) {
			return etoi[id];
		}
	
		ID_in link(ID_ext id, ID_in internal) {
			etoi[id] = internal;
			itoe[internal] = id;
		}
	
		// Returns the items Out_ID and then removes mapping
		ID_in unlink(ID_ext id) {
			itoe.remove(etoi[id]);
			etoi.remove(id);
		}
		
		// Reverse look up. Can be much slower depending on the underlying data holder
		ID_in unlinkByInternal(ID_in internal) {
			etoi.remove(itoe[internal]);
			itoe.remove(internal);
		}
	
		// Re-associates an 
		void relink(In_ID oldid, In_ID newid) {
			etoi[itoe[oldid]] = newid;
		}
	};
};