#include <iostream>
#include "../NZUtils.h"


using namespace std;
using namespace NZUtils;

int main(int argc, char* argv[]) {
	cout << "1+1=2? teach me" << endl
		<< "start learning ..." << endl;
	int ladd, radd, aprs, rcnt(0), wcnt(0);
	for (int i(0); i < 10; ++i) {
		cout << "arg1:(must be 1): " << flush;
		cin >> ladd;
		cout << "arg2:(must be 1): " << flush;
		cin >> radd;
		cout << "ret: " << (ladd + radd) << endl;
		if (NZConfirm("is this ok"))
			++rcnt;
		else
			++wcnt;
		cout << "right: " << rcnt << " wrong: " << wcnt << endl;
	}
	if (rcnt > wcnt)
		cout << "you told me 1+1=2" << endl;
	else if (rcnt < wcnt)
		cout << "you told me 1+1!=2" << endl;
	else
		cout << "you confused me" << endl;
	int term;
	
	if (NZConfirm("are you satisfied with my answer"))
		cout << "thank you, I will keep going" << endl;
	else
		cout << "sorry, but I will try" << endl;

	return 0;
}
