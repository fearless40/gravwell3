#pragma once

//#include "ComponentSystem.h"
//#include <../boost_1_54_0/boost/mpl/sizeof.hpp>
#include <../boost_1_59_0/boost/mpl/assert.hpp>
#include <../boost_1_59_0/boost/mpl/vector.hpp>
#include <../boost_1_59_0/boost/mpl/find_if.hpp>
#include <../boost_1_59_0/boost/mpl/less_equal.hpp>
#include <../boost_1_59_0/boost/mpl/int.hpp>


namespace  mpl = boost::mpl;


// Sizes are in bits! Not bytes
template <class Handle_Type,int GenerationSize, int IndexSize, int UserSize = 0 >
struct Generator_GenericHandle {
	//BOOST_MPL_ASSERT((sizeof(Handle_Type) >= GenerationSize + IndexSize + UserSize));
	
	typedef mpl::vector< unsigned char, unsigned short, unsigned __int32, unsigned __int64 > Size_Types;
	
	// Calculate the min size that the bits can be held in.
	template <typename vec, int sz>
	struct MinTypeSize {
		typedef typename mpl::find_if<Size_Types, mpl::less_equal< mpl::int_<GenerationSize>, mpl::int_<sizeof(mpl::_1) * 8> > >::type iter;
		typedef typename mpl::deref<iter>::type type;
	};
	
	// Type Defs
	typedef Generator_GenericHandle<Handle_Type, GenerationSize, IndexSize, UserSize> type;
	typedef typename MinTypeSize<Size_Types, GenerationSize>::type Generation_Type;
	typedef typename MinTypeSize<Size_Types, IndexSize>::type Index_Type;
	typedef typename MinTypeSize<Size_Types, UserSize>::type User_Type;
	
	// Masks for storing the data
	static const Handle_Type Gen_Mask = (1 << GenerationSize) - 1;
	static const Handle_Type Han_Mask = (1 << IndexSize) - 1;
	static const Handle_Type User_Mask = (1 << UserSize) - 1;


	// Actual Data Storage (just an integer)
	Handle_Type data = 0;

	
	Generator_GenericHandle() {};
	Generator_GenericHandle(Handle_Type inType) : data(inType) { }
	Generator_GenericHandle(Generator_GenericHandle & gh) : data(gh.data) {}
	Generator_GenericHandle(Handle_Type genBits, Handle_Type handBits, Handle_Type userBits = 0) {
		data = ((userBits & User_Mask) << (GenerationSize + IndexSize)) |
			((genBits & Gen_Mask) << (IndexSize)) |
			((handBits & Han_Mask));
	}

	static type make(Handle_Type genBits, Handle_Type handBits, Handle_Type userBits = 0) {
		Handle_Type v = ((userBits & User_Mask) << (GenerationSize + IndexSize)) |
			((genBits & Gen_Mask) << (IndexSize)) |
			((handBits & Han_Mask));
		return type(v);
	}

	Generation_Type generation() {
		return static_cast<Generation_Type>((data >> IndexSize) & Gen_Mask);
	}

	Index_Type index() {
		return static_cast<Index_Type>((data & Han_Mask));
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