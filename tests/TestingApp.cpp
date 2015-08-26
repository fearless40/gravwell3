// TestingApp.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <fstream>
#include "../Gravwell 3/InvasiveRefCount.h"
#include "../Gravwell 3/RefCountPointer.h"

#define nline "\r\n"

#define test( x, text ) if( x ) { file << "Passed " text << (nline); } else { file << "Failed " #text << (nline); }

void test_RefCountPointer(std::ofstream & file) {
	struct tRef : public InvasiveRefCount {
		int w;
	};

	file << "Starting RefCount testing..." << nline;

	RefCountPointer<tRef> w = new tRef();

	if( w->getRef() == 1 ) {
		file << "Passed RefPtr<tRef> w = new tRef()" << nline;
	}
	else {
		file << "Failed RefPtr<tRef> w = new tRef()" << nline;
	}

	RefCountPointer<tRef> w2 = w;

	if( w->getRef() == 2 ) {
		file << "Passed RefPtr<tRef> w2 = w;" << nline;
	}
	else {
		file << "Failed RefPtr<tRef> w2 = w;" << nline;
	}

	w2.release();

	test( !w2 , "w2.release" );

	test( w->getRef() == 1, "w2.release w->getRef() == 1" );

	w2 = w;

	test( w2 == w, "w2 = w" );

	test( w2->getRef() == 2, "w2 == 2 after w2 = w" );

	{
		RefCountPointer<tRef> w3( w );
		test( w3->getRef() == 3, "w2-getRef()==3 after w3(2)");
	}
	
	test( w->getRef() == 2, "w2->getRef()==2 after w3 goes out of scope" );

	w.release();

	test( w2->getRef() == 1, "w2 == 1 after w.release()" );

	w2 = nullptr;

	test( !w2, "w2 = nullptr, w2 == false" );

	

}


int _tmain(int argc, _TCHAR* argv[])
{
	std::ofstream file;
	file.open("test.txt");

	test_RefCountPointer( file );
	
	return 0;
}

