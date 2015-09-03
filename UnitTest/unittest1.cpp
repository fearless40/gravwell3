#include "stdafx.h"
#include "CppUnitTest.h"
#include "../src/util/Work.h"
#include "../src/util/ThreadPool.h"
#include "../src/util/CriticalSection.h"
#include "../src/util/Lock.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Util::Work;

namespace UnitTest
{		
	TEST_CLASS(ThreadPoolTest)
	{
	public:
		
		TEST_METHOD(ThreadPool_Creation)
		{
			// TODO: Your test code here
			int i = 0; 
			Util::Work::ThreadPool pool;

			pool.initalize(1);

			pool.submitWork(wiHelper([](WorkData data) -> void {
				int * a = (int*)data.inData;
				(*a)+=1;
			}, (void *)&i, 0));
		
			Sleep(500);

			Assert::AreEqual(1, i);
		}

		TEST_METHOD(ThreadPool_MultipleThreads)
		{
			int t1 = 0, t2 = 2, t3 = 4, t4 = 6, t5 = 8;
			ThreadPool pool;

			pool.initalize();

			auto adder = [](WorkData data) -> void {
				int * a = (int*)data.inData;
				(*a) += 1;
			};

			pool.submitWork(wiHelper(adder, &t1, nullptr));
			pool.submitWork(wiHelper(adder, &t2, nullptr));
			pool.submitWork(wiHelper(adder, &t3, nullptr));
			pool.submitWork(wiHelper(adder, &t4, nullptr));
			pool.submitWork(wiHelper(adder, &t5, nullptr));

			Sleep(500);

			Assert::AreEqual(1, t1);
			Assert::AreEqual(3, t2);
			Assert::AreEqual(5, t3);
			Assert::AreEqual(7, t4);
			Assert::AreEqual(9, t5);

		}

		TEST_METHOD(ThreadPool_CriticalSection) {
			int t1 = 0;
			Util::CriticalSection cs;
			ThreadPool pool;

			pool.initalize();

			auto lockAdder = [](WorkData data) -> void {
				Util::CriticalSection * cs = reinterpret_cast<Util::CriticalSection*>(data.outData);
				Util::Lock<Util::CriticalSection> lock(*cs);
				int * i = reinterpret_cast<int*>(data.inData);
				(*i) += 1;
			};

			for (int loop = 0; loop < 100; ++loop)
			{
				pool.submitWork(wiHelper(lockAdder, &t1, &cs));
			}

			Sleep(500);

			Assert::AreEqual(100, t1);
		}
		
		TEST_METHOD(ThreadPool_CriticalSection_Failed) {
			int t1 = 0;
			Util::CriticalSection cs;
			ThreadPool pool;

			pool.initalize();

			auto lockAdder = [](WorkData data) -> void {
				
				int k = 0, z = 0;
				int * i = reinterpret_cast<int*>(data.inData);
				k = *i;
				for (int w = k; w < 1000; ++w)
				{
					z += 1;
					//z = z * w;

					if (z > 213455) break;
				}

				k += 1;
				*i = k;
			};

			for (int loop = 0; loop < 100; ++loop)
			{
				pool.submitWork(wiHelper(lockAdder, &t1, &cs));
			}

			Sleep(500);

			Assert::AreEqual(100, t1);
			//Assert::Fail(L"This test will always fail.");
		}

		TEST_METHOD(THreadPool_CriticalSection_Advanced) {
			
			int t1 = 0;
			Util::CriticalSection cs;
			ThreadPool pool;

			pool.initalize();

			auto lockAdder = [](WorkData data) -> void {
				Util::CriticalSection * cs = reinterpret_cast<Util::CriticalSection*>(data.outData);
				Util::Lock<Util::CriticalSection> lock(*cs);
				int k = 0, z = 0;
				int * i = reinterpret_cast<int*>(data.inData);
				k = *i;
				for (int w = k; w < 1000; ++w)
				{
					z += 1;
					//z = z * w;

					if (z > 213455) break;
				}

				k += 1;
				*i = k;
			};

			for (int loop = 0; loop < 100; ++loop)
			{
				pool.submitWork(wiHelper(lockAdder, &t1, &cs));
			}

			Sleep(500);

			Assert::AreEqual(100, t1);
			//Assert::Fail(L"This test will always fail.");
		}
	};
}