#include "codegen.h"

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

MICROCC::CodeGen::CodeGen() {
}

MICROCC::CodeGen::~CodeGen() {
}

int MICROCC::CodeGen::genMidCode(const TokenTable& toktbl, MCodeTable& mctbl) {
	return 0;
}

int MICROCC::CodeGen::genObjCode(const MCodeTable& mctbl, OCodeTable& octbl) {
	return 0;
}
