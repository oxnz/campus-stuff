#include <microcc.h>
//#include <functional>
#include <list>

using namespace std;

int main() {
	struct A {
		A(int a_) : a(a_){}
		int a;
	};

	struct B : public A {
		B(int a, int b_) : A(a), b(b_) {}
		B(const A& a): A(a), b(0){}
		int b;
	};

	list<A> l;
	l.push_front({1});
	l.push_front({2});
	B b(1, 2);
	l.push_front(b);
	A ab = l.front();
	B bb = static_cast<B>(ab);
	cout << "B->A->B: " << bb.a << bb.b << endl;
	while (!l.empty()) {
		A a = l.front();
		l.pop_front();
		cout << a.a << endl;
	}
	return 0;
}
