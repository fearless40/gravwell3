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

	};
}