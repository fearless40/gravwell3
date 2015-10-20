

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

template <class DataType, class Allocator>
class DataHandlerFreeList
{
public:
	typedef uint32_t Index_Type;
	typedef std::pair<Index_Type, Index_Type> Index_Swap;
	typedef std::vector<Index_Swap> SwapList;
	typedef DataType Data_Type;

	static const bool Swaps = false;
	static const bool isDataHandlerClass = true;

protected:

	std::vector<Data_Type> data;
	std::vector<uint32_t> freelist;

public:
	// Returns the index of the added item
	Index_Type add(Data_Type & data) {}
	Index_Type add(Data_Type && data) {}

	// pair_1 = Old Index position
	// pair_2 = New position.
	// Necessary if the class swaps items out rather than just flagging them as empty
	Index_Swap remove(Index_Type index) {}

	Data_Type & get(Data_Type index) {}

	operator Data_Type & [](Index_Type index) {};

	const Data_Type & get(Index_Type index) const {}
	
	Data_Type * getAll() {}

	// Special functions
	// Gets the next index without filling removing it from the list
	Index_Type getNextIndex() {} 

	// Garbage Collect and if items were moved to new locations then 
	// generate the swap list
	SwapList gc() {}
};