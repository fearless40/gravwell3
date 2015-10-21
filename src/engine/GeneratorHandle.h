#pragma once

//#include "ComponentSystem.h"

#include <../boost/mpl/assert.hpp>
#include <../boost/mpl/vector.hpp>
#include <../boost/mpl/find_if.hpp>
#include <../boost/mpl/less_equal.hpp>
#include <../boost/mpl/int.hpp>
#include <cstdint>

namespace  mpl = boost::mpl;


// Sizes are in bits! Not bytes
/*
* Automatic Handle generator 
* The handle is stored as an integer that its size is automatically decided on by the compiler (it picks the smallest)
* /param GenerationSize Number of BITS requested for the generation data
* /param IndexSize Number of BITS requested for the index
* /param UserSize Number of BITS requested for user data. Default is no user data
*/
template <int GenerationSize, int IndexSize, int UserSize = 0 >
struct Generator_GenericHandle {
	BOOST_MPL_ASSERT(( mpl::less_equal< mpl::int_<GenerationSize + IndexSize + UserSize>, mpl::int_<64> > ));
	
	typedef mpl::vector< uint8_t, uint16_t, uint32_t, uint64_t > Size_Types;
	
	// Calculate the min size that the bits can be held in.
	// vec = mpl::sequence type
	// sz = integer representing number of bits requested
	template <typename vec, int sz>
	struct MinTypeSize {
	    typedef mpl::int_<sz> BitSize;
	    
	   template< int item  > 
	    struct compare {
	        template <class T> struct apply {
	            typedef  typename mpl::less_equal< mpl::int_<item>, mpl::int_<sizeof(T)*8> > type;
	        };
	    };
	    
		typedef typename mpl::find_if<Size_Types, compare<sz> >::type iter;
		typedef typename mpl::deref<iter>::type type;
	};
	
	// Type Defs
	typedef Generator_GenericHandle<GenerationSize, IndexSize, UserSize> type;
	typedef typename MinTypeSize<Size_Types, GenerationSize + IndexSize + UserSize>::type Handle_Type;
	typedef typename MinTypeSize<Size_Types, GenerationSize>::type Generation_Type;
	typedef typename MinTypeSize<Size_Types, IndexSize>::type Index_Type;
	typedef typename MinTypeSize<Size_Types, UserSize>::type User_Type;
	
	// Masks for storing the data
	static const Handle_Type Generation_Mask = (1 << GenerationSize) - 1;
	static const Handle_Type Index_Mask = (1 << IndexSize) - 1;
	static const Handle_Type User_Mask = (1 << UserSize) - 1;


	// Actual Data Storage (just an integer)
	Handle_Type data = 0;

	// Constructors
	Generator_GenericHandle() {};
	Generator_GenericHandle(Handle_Type inType) : data(inType) { }
	Generator_GenericHandle(Generator_GenericHandle & gh) : data(gh.data) {}
	Generator_GenericHandle(Handle_Type genBits, Handle_Type handBits, Handle_Type userBits = 0) {
		data = ((userBits & User_Mask) << (GenerationSize + IndexSize)) |
			((genBits & Generation_Mask) << (IndexSize)) |
			((handBits & Index_Mask));
	}

	static type make(Handle_Type genBits, Handle_Type handBits, Handle_Type userBits = 0) {
		Handle_Type v = ((userBits & User_Mask) << (GenerationSize + IndexSize)) |
			((genBits & Generation_Mask) << (IndexSize)) |
			((handBits & Index_Mask));
		return type(v);
	}

	Generation_Type generation() {
		return static_cast<Generation_Type>((data >> IndexSize) & Generation_Mask);
	}

	Index_Type index() {
		return static_cast<Index_Type>((data & Index_Mask));
	}

	User_Type user() {
		return static_cast<User_Type>((data >> (GenerationSize + IndexSize)) & User_Mask);
	}

	bool operator == (const type & comp) {
		return (data == comp.data);
	}

	bool operator != (const type & comp) {
		return (data != comp.data);
	}
	
};
/*
template <class InType, class OutType, class GenSize>
class GeneratorHandle : public Generator<InType,OutType> {
protected:
	GenSize * generations;
	uint32_t size;

public:
	ID_ext create(ID_in value) {
		++generations[value];
		return OutType::create(generations[value], value);
	}
	InType get(OutType) {
		if (generations[OutType::getIndex(OutType)] = OutType::getGenerationData(OutType))
			return OutType::getIndex(OutType);
	}
	OutType remove(InType value) {
		++generations[value];
	}
	OutType find(InType value) {
		return OutType::create(generations[value], value);
	}
};*/