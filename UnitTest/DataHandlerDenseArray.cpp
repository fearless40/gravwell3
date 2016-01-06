#include "stdafx.h"
#include "CppUnitTest.h"
#include <type_traits>
#include <cstdint>
#include <../boost/mpl/assert.hpp>
#include "../src/engine/DataHandlerDenseArray.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
namespace cs = ComponentSys;

namespace UnitTest
{
	TEST_CLASS(ID_TEST)
	{
	public:
		typedef ComponentSys::DataHandlerDenseArray< int > Data;

		TEST_METHOD(DataHandlerDenseArray_Initialization) {
			ComponentSys::DataHandlerDenseArray< int > x;
		}

		TEST_METHOD(DataHandlerDenseArray_Add_Item) {
			Data x;
			Data::Index_Type index;

			index = x.add(32);
			index = x.add(33);
			Assert::AreEqual(static_cast<Data::Index_Type>(2), index);
		}

		TEST_METHOD(DataHandlerDenseArray_get) {
			Data x;
			Data::Index_Type index;

			x.add(32);
			index = x.add(33);
			Assert::AreEqual(33, x.get(index));
		}

		TEST_METHOD(DataHandlerDenseArray_remove_first_item_swap_check_second) {
			Data x;
			Data::Index_Type index;
			Data::Index_Swap iSwap;

			index = x.add(32);
			x.add(33);
			iSwap = x.remove(index);
			Assert::AreEqual(static_cast<Data::Index_Type>(0), iSwap.second);

		}

		TEST_METHOD(DataHandlerDenseArray_remove_first_item_swap_check_first) {
			Data x;
			Data::Index_Type index;
			Data::Index_Swap iSwap;

			index = x.add(32);
			x.add(33);
			iSwap = x.remove(index);
			Assert::AreEqual(static_cast<Data::Index_Type>(1), iSwap.first);
		}

		TEST_METHOD(DataHandlerDenseArray_get_operator) {
			Data x;
			Data::Data_Type d;
			Data::Index_Type index;
			Data::Index_Swap iSwap;

			index = x.add(32);
			x.add(33);
			d = x[index];
			Assert::AreEqual(32, d);
		}



	};
}