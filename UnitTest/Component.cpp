#include "stdafx.h"
#include "CppUnitTest.h"
#include <type_traits>
#include <cstdint>
#include <../boost/mpl/assert.hpp>
#include "../src/engine/DataHandlerDenseArray.h"
#include "../src/engine/LinkHash.h"
#include "../src/engine/Component.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
namespace cs = ComponentSys;

namespace UnitTest
{
	TEST_CLASS(ID_TEST)
	{
	public:



		typedef cs::DataHandlerDenseArray< int > Data;
		typedef cs::LinkHash<int, Data::Index_Type> Link;
		typedef cs::Component< Data, Link > Comp;

		TEST_METHOD(Component_Initialization) {

			cs::Component< Data, Link > x;
		}

		TEST_METHOD(Component_Add_Item) {
			
			Comp x;
			Comp::Index_Type index = 1;
			Comp::Data_Type data = 10;

			x.add(index, data);
			
			Assert::AreEqual(static_cast<const Comp::Data_Type>(10), x.get(index));
		}

		TEST_METHOD(Component_get) {
			Comp x;
			Comp::Index_Type index = 1;
			Comp::Data_Type d = 2;

			x.add(1,d);
			x.add(2, { 3 });
			x.add(3, { 4 });

			Assert::AreEqual(4, x.get(3));
		}

		TEST_METHOD(Component_remove_first_item_swap_check_second) {
			Comp x;
			
			x.add(1, { 1 });
			x.add(2, { 2 });
			x.add(3, { 3 });
			x.remove(2);

			Assert::AreEqual(3, x.get(3));

		}

		TEST_METHOD(Component_update_item) {
			Comp x;

			x.add(1, { 1 });
			x.add(2, { 2 });
			x.add(3, { 3 });

			x.update(2, 4);

			Assert::AreEqual(4, x.get(2));
		}

		/*TEST_METHOD(DataHandlerDenseArray_get_operator) {
			Data x;
			Data::Data_Type d;
			Data::Index_Type index;
			Data::Index_Swap iSwap;

			index = x.add(32);
			x.add(33);
			d = x[index];
			Assert::AreEqual(32, d);
		}*/


	};
}