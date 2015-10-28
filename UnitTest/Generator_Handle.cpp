#include "stdafx.h"
#include "CppUnitTest.h"
#include <type_traits>
#include <cstdint>
#include <../boost/mpl/assert.hpp>
#include "../src/engine/Handle.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
namespace cs = ComponentSys;

namespace UnitTest
{
	TEST_CLASS(ThreadPoolTest)
	{
	public:
		template <class T>
		struct _x {
			
			T add(T y) {
				return y;
			}
		};

		template <>
		struct _x<void> {
			void add(void) {
				return;
			}
		};

		template <class T>
		struct x : _x<T>{

		};


		TEST_METHOD(Generator_Handle_Compiler_Test)
		{
			typedef cs::Handle<16, 16> Handle1;
			Handle1 g1;
			BOOST_MPL_ASSERT((std::is_same < Handle1::Generation_Type, std::uint16_t>));
			BOOST_MPL_ASSERT((std::is_same < Handle1::Index_Type, std::uint16_t>));

			typedef cs::Handle<4, 22> Handle2;
			Handle2 g2;
			BOOST_MPL_ASSERT((std::is_same < Handle2::Generation_Type, std::uint8_t>));
			BOOST_MPL_ASSERT((std::is_same < Handle2::Index_Type, std::uint32_t>));
			Assert::IsTrue(true, L"Compiler passed");
		}

		TEST_METHOD(Generator_Handle_RuntimeTest_Generation)
		{
			typedef cs::Handle<8, 24> Handle;
			Handle h1(4, 88990);
			Assert::AreEqual(h1.generation(), static_cast<Handle::Generation_Type>(4));
		}

		TEST_METHOD(Generator_Handle_RuntimeTest_Index)
		{
			typedef cs::Handle<8, 24> Handle;
			Handle h1(4, 88990);
			Assert::AreEqual(h1.index(), static_cast<Handle::Index_Type>(88990));
		}

		TEST_METHOD(Generator_Handle_RuntimeTest_User)
		{
			typedef cs::Handle<8, 24, 4> Handle;
			Handle h1(4, 88990, 3);
			Assert::AreEqual(h1.user(), static_cast<Handle::User_Type>(3));
		}

		TEST_METHOD(Lets_See_What_The_Compiler_Can_DO)
		{
			

			int v = 0;
			x<int> test1;
			x<void> test2;
			v = test1.add(1);

			test2.add();

		}
	};
}