#pragma once

#include "ComponentSystem.h"

template < class IDType, class OutTransform  >
struct LinkByNone : public Linker{
public:
	typedef IDType In_ID;
	typedef OutTransform Out_ID;
	const static bool isLinkEmpty = false;
};

template<>
struct LinkByNone<void, void> {
	const static bool isLinkEmpty = true;
};