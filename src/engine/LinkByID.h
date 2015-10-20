
#pragma once

#include <hash_map>


template < class IDType, class OutTransform  >
class LinkByID {
public:
	typedef IDType In_ID;
	typedef OutTransform Out_ID;

	const static bool isLinkClass = true;

protected:
	std::hash_map< In_ID, Out_ID >  mapper;
	
	Out_ID get(In_ID id) {}

	Out_ID add(In_ID  id, Out_ID nextEmptyOutID) {}

	// Returns the items Out_ID and then removes mapping
	Out_ID remove(In_ID id) {}

	// Re-associates an 
	//void move(In_ID oldid, In_ID newid) {}

};