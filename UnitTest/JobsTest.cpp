#include "stdafx.h"
#include "CppUnitTest.h"
#include "../src/util/Work.h"
#include "../src/util/ThreadPool.h"
#include "../src/util/CriticalSection.h"
#include "../src/util/Lock.h"
#include "../src/util/Job.h"
#include "../src/util/Jobs.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Util::Work;

namespace UnitTest
{
	TEST_CLASS(JobsTest)
	{
	public:

		TEST_METHOD(Jobs_SimpleActivate)
		{
			Util::Work::ThreadPool pool;
			Util::Work::Jobs jobs(pool);
			int i = 0;

			pool.initalize();

			jobs.createSubmit(wiHelper([](WorkData data) -> void {
				int * a = (int*)data.inData;
				(*a) += 1;
			}, &i, 0));

			Sleep(500);

			Assert::AreEqual(1, i);
		};

		TEST_METHOD(Jobs_AdvanceActivate)
		{
			Util::Work::ThreadPool pool;
			Util::Work::Jobs jobs(pool);
			char word[32] = "";
			int  let[5] = { 0,1,2,3,4 };
			
			auto addText = [](WorkData data) -> void {
				char * str = reinterpret_cast<char *> (data.inData);
				int  * letter = reinterpret_cast<int *>(data.outData);

				char adder[4] = "";

				switch (*letter) {
				case 0: strcat(str, "H"); break;
				case 1: strcat(str, "e"); break;
				case 2: strcat(str, "l"); break;
				case 3: strcat(str, "l"); break;
				case 4: strcat(str, "o"); break;
				};
			};

			pool.initalize();

			Util::Work::Job * j0, *j1, *j2, *j3, *j4;
			j0 = new Job(wiHelper(addText, word, &let[0]));
			j1 = new Job(wiHelper(addText, word, &let[1]));
			j2 = new Job(wiHelper(addText, word, &let[2]));
			j3 = new Job(wiHelper(addText, word, &let[3]));
			j4 = new Job(wiHelper(addText, word, &let[4]));

			j0->addDependent(j1);
			j1->addDependent(j2);
			j2->addDependent(j3);
			j3->addDependent(j4);
			
			jobs.submit(j0);

			Sleep(500);

			Assert::AreEqual("Hello", word);

		}

		TEST_METHOD(Jobs_ComplicatedUse) 
		{
			Util::Work::ThreadPool pool;
			Util::Work::Jobs jobs(pool);
			Util::CriticalSection cs;

			char word[32] = "";
			int  let[5] = { 0,1,2,3,4 };
			int  adderResult = 0;

			auto addText = [](WorkData data) -> void {
				char * str = reinterpret_cast<char *> (data.inData);
				int  * letter = reinterpret_cast<int *>(data.outData);

				char adder[4] = "";

				switch (*letter) {
				case 0: strcat(str, "H"); break;
				case 1: strcat(str, "e"); break;
				case 2: strcat(str, "l"); break;
				case 3: strcat(str, "l"); break;
				case 4: strcat(str, "o"); break;
				};
			};

			auto lockAdder = [](WorkData data) -> void {
				Util::CriticalSection * cs = reinterpret_cast<Util::CriticalSection*>(data.outData);
				Util::Lock<Util::CriticalSection> lock(*cs);
				int k = 0, z = 0;
				int * i = reinterpret_cast<int*>(data.inData);
				k = *i;
				for (int w = 0; w < k*20000; ++w)
				{
					z += 1;
					//z = z * w;

					if (z > 213455) break;
				}

				k += 1;
				*i = k;
			};

			pool.initalize();

			Util::Work::Job * j0, *j1, *j2, *j3, *j4;
			Util::Work::Job * j0_1, *j0_2, *j0_3, *j1_1, *j2_1, *j3_1, *j3_2, *j3_3, *j4_1, *j4_2;
			j0 = new Job(wiHelper(addText, word, &let[0]));
			j1 = new Job(wiHelper(addText, word, &let[1]));
			j2 = new Job(wiHelper(addText, word, &let[2]));
			j3 = new Job(wiHelper(addText, word, &let[3]));
			j4 = new Job(wiHelper(addText, word, &let[4]));

			j0_1 = new Job(wiHelper(lockAdder, &adderResult, &cs));
			j0_2 = new Job(wiHelper(lockAdder, &adderResult, &cs));
			j0_3 = new Job(wiHelper(lockAdder, &adderResult, &cs));
			j1_1 = new Job(wiHelper(lockAdder, &adderResult, &cs));
			j2_1 = new Job(wiHelper(lockAdder, &adderResult, &cs));
			j3_1 = new Job(wiHelper(lockAdder, &adderResult, &cs));
			j3_2 = new Job(wiHelper(lockAdder, &adderResult, &cs));
			j3_3 = new Job(wiHelper(lockAdder, &adderResult, &cs));
			j4_1 = new Job(wiHelper(lockAdder, &adderResult, &cs));
			j4_2 = new Job(wiHelper(lockAdder, &adderResult, &cs));

			j0->addDependent(j1);
			j0->addDependent(j0_1);
			j0->addDependent(j0_2);
			j0->addDependent(j0_3);

			j1->addDependent(j2);
			j1->addDependent(j1_1);

			j2->addDependent(j3);
			j2->addDependent(j2_1);

			j3->addDependent(j4);
			j3->addDependent(j3_1);
			j3->addDependent(j3_2);
			j3->addDependent(j3_3);

			j4->addDependent(j4_1);
			j4->addDependent(j4_2);

			jobs.submit(j0);

			Sleep(500);

			Assert::AreEqual("Hello", word);
			Assert::AreEqual(10, adderResult);
		}

		TEST_METHOD(Job_Fail_Advanced)
		{
			Util::Work::ThreadPool pool;
			Util::Work::Jobs jobs(pool);
			char word[32] = "";
			int  let[5] = { 0,1,2,3,4 };

			auto addText = [](WorkData data) -> void {
				char * str = reinterpret_cast<char *> (data.inData);
				int  * letter = reinterpret_cast<int *>(data.outData);

				char adder[4] = "";

				switch (*letter) {
				case 0: strcat(str, "H"); break;
				case 1: strcat(str, "e"); break;
				case 2: strcat(str, "l"); break;
				case 3: strcat(str, "l"); break;
				case 4: strcat(str, "o"); break;
				};
			};

			pool.initalize();

			Util::Work::Job * j0, *j1, *j2, *j3, *j4;
			j0 = new Job(wiHelper(addText, word, &let[0]));
			j1 = new Job(wiHelper(addText, word, &let[1]));
			j2 = new Job(wiHelper(addText, word, &let[2]));
			j3 = new Job(wiHelper(addText, word, &let[3]));
			j4 = new Job(wiHelper(addText, word, &let[4]));

			j0->addDependent(j1);

			jobs.submit(j0);
			jobs.submit(j2);
			jobs.submit(j3);
			jobs.submit(j4);

			Sleep(500);

			Assert::AreEqual("Hello", word);

		}
	};
}