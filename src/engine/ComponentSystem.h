#pragma once


template <class ID_Type, class IDTransform_Type>
struct Linker {
	typedef ID_Type ID_in;
	typedef IDTransform_Type ID_out;
	const static bool isLinkClass = true;
};

template <class ID_Internal, class ID_External>
struct Generator {
	typedef ID_Internal	ID_in;
	typedef ID_External ID_ext;
	static const bool isGeneratorClass = true;
};

struct Component {
	static const bool isComponentClass = true;
};

