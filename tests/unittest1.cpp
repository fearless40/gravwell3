#include "stdafx.h"
#include "CppUnitTest.h"
#include "../src/game-gwell/Coord.h"


namespace Microsoft {
	namespace VisualStudio {
		namespace CppUnitTestFramework {
			template<> inline std::wstring ToString<long long>(const long long& t) { RETURN_WIDE_STRING(t); }
		}
	}
}

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Game;



namespace tests
{		
	TEST_CLASS(GameCoord)
	{
	public:
		
		TEST_METHOD(GameCoord_Get)
		{
			Coord c{ 1l,0 };
			Assert::AreEqual(static_cast<Coord::int_type>(1), c.asInt());
			// TODO: Your test code here
		}

		TEST_METHOD(GameCoord_Add) {
			Coord x1{ 1,0 };
			Coord x2{ 1,0 };
			auto x3 = x1 + x2;
			Assert::AreEqual(static_cast<Coord::int_type>(2), x3.asInt());
		}

		TEST_METHOD(GameCoord_AddDec_Float) {
			Coord x1{ 0,1 };
			Coord x2{ 0,1 };
			auto x3 = x1 + x2;
			Assert::AreEqual((2.0f / 16.0f), x3.asFloat());
		}

		TEST_METHOD(GameCoord_AddDec_Double) {
			Coord x1{ 0,1 };
			Coord x2{ 0,1 };
			auto x3 = x1 + x2;
			Assert::AreEqual((2.0 / 16.0), x3.asDouble());
		}

		TEST_METHOD(GameCoord_AddDec_CheckInt) {
			Coord x1{ 0,1 };
			Coord x2{ 0,1 };
			auto x3 = x1 + x2;
			Assert::AreEqual((Coord::int_type)0, x3.asInt());
		}
		
	};
}