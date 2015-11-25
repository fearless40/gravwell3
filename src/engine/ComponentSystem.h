#pragma once

namespace ComponentSys {

	template <class ID_Type, class IDTransform_Type>
	struct LinkerTraits {
		typedef ID_Type ID_ext;
		typedef IDTransform_Type ID_in;
		const constexpr bool isLinkClass = true;
	};

	template <class ID_Internal, class ID_External>
	struct GeneratorTraits {
		typedef ID_Internal	ID_in;
		typedef ID_External ID_ext;
		static constexpr bool isGeneratorClass = true;

		/*
		* Generators must have the following functions for them to work
		* ID_ext create( ID_in )
		* ID_in get(ID_ext)
		* ID_in remove(ID_ext)
		*/

	};

	struct ComponentTraits {
		static constexpr bool isComponentClass = true;
	};

	template< class DataType, class IndexType, bool MovesMemory = false >
	struct DataHandlerTraits {

	};

	template< class DataType, class IndexType>
	struct DataHandlerTraits<DataType,IndexType,true> {
		typedef DataType Data_Type;
		typedef IndexType Index_Type;
		typedef std::pair<Index_Type, Index_Type> Index_Swap;		//first = oldIndex, second = newIndex
		typedef std::vector<Index_Swap> SwapList;
		static constexpr bool dataHandlerMovesMemory = true;
		static constexpr bool isDataHandlerClass = true;
	};

	template< class DataType, class IndexType>
	struct DataHandlerTraits<DataType, IndexType, false> {
		typedef DataType Data_Type;
		typedef IndexType Index_Type;
		static constexpr bool dataHandlerMovesMemory = false;
		static constexpr bool isDataHandlerClass = true;
	};

	enum class ViewMemoryUse {
		Copy,
		Pointer
	};

	template <ViewMemoryUse VMU>
	struct ViewTraits {
		static constexpr bool isView = true;
		static constexpr bool ownsMemory = std::is_same<VMU, ViewMemoryUse::Copy>::value;
	};

	struct NullAllocator {};



};