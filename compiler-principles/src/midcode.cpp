#include "midcode.h"

void start(void) {
	/* semantic initializations, none needed */
}

void finish(void) {
	/* generate code to finish program */
	//cout << "Halt" << endl;
}

using namespace std;

const char* opname[] = {
	"add",
	"sub",
	"mul",
	"div",
	"mod",
	"push",
	"pop",
};

ostream& MICROCC::operator<<(ostream& os, const MICROCC::MidCode& mc) {
	os << opname[static_cast<uint8_t>(mc.op)] << "("
		<< mc.operand1 << ", " << mc.operand2 << ") => " << mc.result;
	return os;
}
