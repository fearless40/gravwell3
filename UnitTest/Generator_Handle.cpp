#include "stdafx.h"
#include "CppUnitTest.h"
#include "../src/engine/GeneratorHandle.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest
{
	TEST_CLASS(ThreadPoolTest)
	{
	public:

		TEST_METHOD(Generator_Handle_Compiler_Test)
		{
			Generator_GenericHandle<unsigned __int32, 16, 16, 0> g1;
		}
	};
}