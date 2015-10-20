#pragma once



template < class IDType, class OutTransform  >
struct LinkByNone {
public:
	typedef IDType In_ID;
	typedef OutTransform Out_ID;

	const static bool isLinkClass = true;
	const static bool isLinkEmpty = false;
};

template<>
struct LinkByNone<void, void> {
	const static bool isLinkEmpty = true;
};