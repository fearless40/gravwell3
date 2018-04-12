
#pragma once

#include <unordered_map>

#include "ComponentSystem.h"

namespace ComponentSys {

	template < class ID_External, class ID_Internal  >
	class LinkHash    {
		
	public:
		typedef LinkerTraits<ID_External, ID_Internal> traits;
		typedef typename traits::ID_ext ID_ext;
		typedef typename traits::ID_in	ID_in;

	protected:
		std::unordered_map<ID_External, ID_Internal>  etoi;
		std::unordered_map<ID_Internal, ID_External>  itoe;



	public:
		ID_in get(const ID_ext id) const {
			return etoi.at(id);
		}
	
		void link(const ID_ext id, const ID_in internal) {
			etoi[id] = internal;
			itoe[internal] = id;
		}
	
		// Returns the items Out_ID and then removes mapping
		ID_in unlink(const ID_ext id) {
			ID_Internal id_in = etoi[id];
			itoe.erase(etoi[id]);
			etoi.erase(id);
			return id_in;
		}
		
		// Reverse look up. Can be much slower depending on the underlying data holder
		void unlinkByInternal(const ID_in internal) {
			etoi.erase(itoe[internal]);
			itoe.erase(internal);
		}
	
		// Re-associates an 
		void relink(const ID_in oldid, const ID_in newid) {
			etoi[itoe[oldid]] = newid;
		}

		std::size_t size() const {
			return etoi.size();
		}

		// Purely for testing purposes 
		std::size_t size2() const {
			return itoe.size();
		}
	};
};