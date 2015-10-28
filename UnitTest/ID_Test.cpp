#include "stdafx.h"
#include "CppUnitTest.h"
#include <type_traits>
#include <cstdint>
#include <../boost/mpl/assert.hpp>
#include "../src/engine/ID.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
namespace cs = ComponentSys;

namespace UnitTest
{
	TEST_CLASS(ID_TEST)
	{
	public:

		TEST_METHOD(ID_Compiler_Test) {
			cs::ID<int> id1 = { 16 }, id2 = { 32 }, id3 = { 64 };
			cs::ID<int>::unordered_map<int> hashmap;
			hashmap[id1] = 1;
			hashmap[id2] = 2;
			hashmap[id3] = 3;
		}

		TEST_METHOD(ID_Basic_Hash_Test) {
			cs::ID<int> id1 = { 16 }, id2 = { 32 }, id3 = { 64 };
			cs::ID<int>::unordered_map<int> hashmap;
			hashmap[id1] = 1;
			hashmap[id2] = 2;
			hashmap[id3] = 3;
			Assert::AreEqual(hashmap[id2], 2);
		}

		TEST_METHOD(ID_Basic_Data_Test) {
			cs::ID<int,32> id1 = { 16 }, id2 = { 32 }, id3 = { 64 };
			const cs::ID<int, 32>::Value_Type check = 16;
			
			Assert::AreEqual( id1.value,  check );
		}

		TEST_METHOD(ID_Compiler_error_uncomment_to_test) {
			cs::ID<int> id1 = { 16 }, id2 = { 32 }, id3 = { 64 };
			cs::ID<long> id5 = { 16 };
			//id1 == id5; //should cause compiler error
		}

	};
}