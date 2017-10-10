

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

	template <class data>
	class DataHandlerDenseArray  
	{
	public:
		typedef DataHandlerTraits<data, std::size_t, true> traits;
		typedef typename traits::Data_Type Data_Type;
		typedef typename traits::Index_Type Index_Type;
		typedef typename traits::Index_Swap Index_Swap;
		
	protected:
		std::vector<Data_Type> data;
		

	public:
		// Returns the index of the added item
		Index_Type add(const Data_Type & item) {
			data.push_back(item);
			return data.size() - 1;
		}
		/*Index_Type add(Data_Type && data) {
			data.emplace_back(data);
			return data.size() - 1;
		}*/

		// pair_1 = Old Index position
		// pair_2 = New position.
		// Necessary if the class swaps items out rather than just flagging them as empty
		Index_Swap remove(const Index_Type index) {
			Index_Swap ret  { data.size()-1, index };
			data[index] = data[ret.first];
			data.erase(data.end()); //Update the internal vector iterators
			return ret;
		}

		Data_Type get(const Index_Type index) const {
			return data[index];
		}

		Data_Type operator [](const Index_Type index) const {
			return data[index];
		}

		void edit(const Index_Type index, const Data_Type & dt) {
			data[index] = dt;
		}

		const Data_Type & getConst(const Index_Type index) const {
			return data[index];
		}

		/*template <ViewMemoryUse VT>
		auto getAll() -> ViewArray<Data_Type,VT>{
		}*/

	};
}