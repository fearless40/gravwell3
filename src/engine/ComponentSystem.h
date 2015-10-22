#pragma once

namespace ComponentSys {

	template <class ID_Type, class IDTransform_Type>
	struct LinkerTraits {
		typedef ID_Type ID_ext;
		typedef IDTransform_Type ID_in;
		const static bool isLinkClass = true;
	};

	template <class ID_Internal, class ID_External>
	struct GeneratorTraits {
		typedef ID_Internal	ID_in;
		typedef ID_External ID_ext;
		static const bool isGeneratorClass = true;

		/*
		* Generators must have the following functions for them to work
		* ID_ext create( ID_in )
		* ID_in get(ID_ext)
		* ID_in remove(ID_ext)
		*/

	};

	struct ComponentTraits {
		static const bool isComponentClass = true;
	};

};