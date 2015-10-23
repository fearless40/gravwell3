#pragma once
#include <functional>

namespace ComponentSys {
    
	/// Very simple class representing an opaque value
	// Template Parameter: Type is only used by the compiler to prevent ID from being 
	//		crossed into different classes. 

    template <class Type> 
	struct ID {
		int value;
	};

	template< class Type>
	bool operator == (const ID<Type> & id1, const ID<Type> & id2) {
		return id1.value == id2.value;
	}

}

namespace std {
	template<class Type>
	size_t hash < ID<Type> > (ID<Type> id ) {
		return std::hash(id.value);
	};
};