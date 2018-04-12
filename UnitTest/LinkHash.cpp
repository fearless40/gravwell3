#include "stdafx.h"
#include "CppUnitTest.h"
#include <type_traits>
#include <cstdint>
#include <../boost/mpl/assert.hpp>
#include "../src/engine/LinkHash.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
namespace cs = ComponentSys;

namespace UnitTest
{
	TEST_CLASS(LinkHash_Test)
	{
	public:



		typedef cs::LinkHash< int, int > Linker;

		TEST_METHOD(LinkHash_Initalization) {

			cs::LinkHash< int, int > x;
			Assert::AreEqual(true, true);
		}

		TEST_METHOD(LinkHash_link) {
			Linker x;
			x.link(1, 1);
			x.link(2, 2);
			x.link(3, 3);
			
			Assert::AreEqual(x.size(), x.size2());
		}

		TEST_METHOD(LinkHash_Get) {
			Linker x;
			x.link(1, 1);
			x.link(2, 2);
			x.link(3, 3);

			Assert::AreEqual(1, x.get(1));
		}

		TEST_METHOD(LinkHash_unlink_returnValue) {
			Linker x;
			x.link(1, 1);
			x.link(2, 2);
			x.link(3, 3);

			Linker::ID_in in = x.unlink(1);

			Assert::AreEqual(1,in);
		}

		TEST_METHOD(LinkHash_unlink_size_etoi) {
			Linker x;
			x.link(1, 1);
			x.link(2, 2);
			x.link(3, 3);

			Linker::ID_in in = x.unlink(1);

			Assert::AreEqual(static_cast<std::size_t>(2), x.size());
		}

		TEST_METHOD(LinkHash_unlink_size_itoe) {
			Linker x;
			x.link(1, 1);
			x.link(2, 2);
			x.link(3, 3);

			Linker::ID_in in = x.unlink(1);

			Assert::AreEqual(static_cast<std::size_t>(2), x.size2());
		}

		TEST_METHOD(LinkHash_unlink_checkremaining_values) {
			Linker x;
			x.link(1, 1);
			x.link(2, 2);
			x.link(3, 3);

			Linker::ID_in in = x.unlink(1);

			Assert::AreEqual(2, x.get(2));
			Assert::AreEqual(3, x.get(3));
		}

		TEST_METHOD(LinkHash_unlink_byInternal) {
			Linker x;
			x.link(1, 1);
			x.link(2, 2);
			x.link(3, 3);

			x.unlinkByInternal(1);

			Assert::AreEqual(x.size(), x.size2());
		}

		TEST_METHOD(LinkHash_relink) {
			Linker x;
			x.link(1, 1);
			x.link(2, 2);
			x.link(3, 3);

			x.relink(1, 4);

			Assert::AreEqual(1, x.get(4));
		}


	};
}