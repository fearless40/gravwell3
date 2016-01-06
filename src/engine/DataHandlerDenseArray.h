

#pragma once

/*
template <class DataType>
class DataHandlerFreeList {

public:
typedef IndexType uint32_t;

protected:
DataType * mBegin;
uint32_t * mFreeList;
size_t	   mMaxItems;

public:
void add(DataType & type);
void remove(IndexType index);
DataType * getAll();
void get(IndexType index);

*/

#include <algorithm>
#include <vector>
#include "ComponentSystem.h"
namespace ComponentSys {

	template <class DataType /*, class Allocator*/>
	class DataHandlerDenseArray : public DataHandlerTraitsMoves<DataType, std::size_t>
	{
	protected:
		std::vector<Data_Type> data;
		

	public:
		// Returns the index of the added item
		Index_Type add(const Data_Type & data) {
			data.push_back(data);
			return data.size() - 1;
		}
		/*Index_Type add(Data_Type && data) {
			data.emplace_back(data);
			return data.size() - 1;
		}*/

		// pair_1 = Old Index position
		// pair_2 = New position.
		// Necessary if the class swaps items out rather than just flagging them as empty
		Index_Swap remove(Index_Type index) {
			Index_Swap ret  { data.size()-1, index };
			data[index] = data[ret.first];
			return ret;
		}

		Data_Type & get(Data_Type index) {
			return data[index];
		}

		Data_Type & operator [](Index_Type index) {
			return data[index];
		}

		const Data_Type & getConst(Index_Type index) const {
			return data[index];
		}

		/*template <ViewMemoryUse VT>
		auto getAll() -> ViewArray<Data_Type,VT>{
		}*/

	};
}