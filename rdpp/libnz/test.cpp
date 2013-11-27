#include <iostream>
#include <new>
#include <cstring>

using namespace std;

class HasPointers {
	struct MyData {
		const char* theString;
		const int* theInts;
		size_t numInts;
		MyData(const char* pString, const int* pInts, size_t nInts)
			: theString(pString), theInts(pInts), numInts(nInts) {}
	} *theData;
	static MyData* clone(const char *otherString,
			const int *otherInts, size_t nInts) {
		char *newChars = new char[strlen(otherString)+1];
		int *newInts;
		try {
			newInts = new int[nInts];
		} catch(bad_alloc&) {
			delete[] newChars;
			throw;
		}
		try {
			strcpy(newChars, otherString);
			for (size_t i = 0; i < nInts; ++i)
				newInts[i] = otherInts[i];
		} catch (...) {
			delete [] newInts;
			delete [] newChars;
			throw;
		}
		return new MyData(newChars, newInts, nInts);
	}
	static MyData* clone(const MyData* otherData) {
		return clone(otherData->theString,
				otherData->theInts, otherData->numInts);
	}

	static void cleanup(const MyData* theData) {
		delete [] theData->theString;
		delete [] theData->theInts;
		delete theData;
	}
public:
	HasPointers(const char* someString, const int* someInts, size_t numInts) {
		theData = clone(someString, someInts, numInts);
	}
	HasPointers(const HasPointers& source) {
		theData = clone(source.theData);
	}
	HasPointers& operator=(const HasPointers& rhs) {
		if (this != &rhs) {
			MyData* newData = clone(rhs.theData->theString,
					rhs.theData->theInts, rhs.theData->numInts);
			cleanup(theData);
			theData = newData;
		}
		return *this;
	}
	~HasPointers() { cleanup(theData); }
	friend ostream& operator<< (ostream& os, const HasPointers& obj) {
		os << obj.theData->theString << ": ";
		for (size_t i = 0; i < obj.theData->numInts; ++i)
			os << obj.theData->theInts[i] << ' ';
		return os;
	}
};

int test_01() {
	int someNums[] = {1, 2, 3, 4};
	size_t someCount = sizeof someNums / sizeof someNums[0];
	int someMoreNums[] = {5, 6, 7};
	size_t someMoreCount = sizeof someMoreNums / sizeof someMoreNums[0];
	HasPointers h1("hello", someNums, someCount);
	HasPointers h2("Goodbye", someMoreNums, someMoreCount);
	cout << h1 << endl;
	h1 = h2;
	cout << h1 << endl;
	return 0;
}

//#define NDEBUG
#include "NZUtils.h"

int test_str() {
	string str = ("你好");
	cout << str << ' ' << str.length() << endl
		<< str[0] << str[1] << str[2] << endl;
	locale def;
	cout << def.name() << endl;
	wstring str2 = L"你好";
	wcout << "Wchar: " << str2.length() << str2[1] << endl;
	return 0;
}

#include <iomanip>

int test_os() {
	char c = cout.fill('0');
	NZAssert(c == '0');
	cout << setw(4) << 1 << endl;
	return 0;
}

#include "NZFinder.h"
#include <vector>
int test_finder() {
	try {
		NZ::NZFinder finder("./", "e");
		dirent* dp;
		while ((dp = finder.find())) {
			cout << dp->d_name << endl;
		}
	} catch (const NZ::NZFinder::NZRegexExcept& e) {
		cout << "RegexException: " << e.what() << endl;
	} catch (const exception& e) {
		cout << "Exception: " << e.what() << endl;
	} catch (...) {
		cout << "Error occured" << endl;
	}
	return 0;
}

#include "NZLogger.h"
int test_NZLogger() {
	using NZ::NZLogger;
	NZLogger::setLogLevel(NZLogger::LogLevel::INFO);
	NZLog(NZLogger::LogLevel::DEBUG, "HELLO %s %c", "world", '!');
	NZLog(NZLogger::LogLevel::INFO, "HELLO %s %c", "world", '!');
	NZLog(NZLogger::LogLevel::WARNING, "HELLO");
	NZLog(NZLogger::LogLevel::ERROR, "HELLO");
	NZLog(NZLogger::LogLevel::FATAL, "HELLO");
	NZLogger::m_color = false;
	NZLog(NZLogger::LogLevel::FATAL, "HELLO");
	return 0;
}

int test_confirm() {
	if (NZUtils::NZConfirm("Delete file: abc.test")) {
		cout << "YES" << endl;
	} else
		cout << "NO" << endl;
	return 0;
}

int test_hexdump() {
	const unsigned char a[] = "abcdefghijklmnopqrstuvwxyzabcdefghijkl"
		"mnopqrstuvwxyzabc\00\01\02\12\x55\xFF";
	cout << "tested " << setw(8) << sizeof(a)/sizeof(*a) << " elements" << endl;
	NZUtils::NZHexDump(a, sizeof(a)/sizeof(*a), 16, 2, false);
	cout << "tested " << setw(8) << sizeof(a)/sizeof(*a) << " elements" << endl;
	return 0;
}

int main() {
	return test_hexdump();
	test_NZLogger();
	//test_confirm();
	test_finder();
	NZAssert(1 == 1);
	test_os();
}
