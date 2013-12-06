#include <microcc.h>
//#include <functional>

using namespace std;

int main() {
	struct ReduceItem {
		int a;
		int b;
		function<void(int i)> gen;
	};
	ReduceItem ReduceTable[5] = {
		{10, 10, [](int i) { cout << "hello" << i << endl; }},
		{10, 10, [](int i) { cout << "hello" << i << endl; }},
		{10, 10, [](int i) { cout << "hello" << i << endl; }},
		{10, 10, [](int i) { cout << "hello" << i << endl; }},
		{10, 10, [](int i) { cout << "hello" << i << endl; }},
	};

	for (int i = 0; i < 5; ++i)
		ReduceTable[i].gen(i);
	return 0;
}
