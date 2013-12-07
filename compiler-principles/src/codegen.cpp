#include "codegen.h"
#include "parser.h"

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

ostream&
MICROCC::operator<<(ostream& os, const MICROCC::MidCode& mc) {
	os << opname[static_cast<uint8_t>(mc.op)] << "("
		<< mc.operand1 << ", " << mc.operand2 << ") => " << mc.result;
	return os;
}

ostream&
MICROCC::operator<<(ostream& os, const MICROCC::MCodeTable& mctbl) {
	os << "MidCode Table (size: " << mctbl.size() << "):" << endl;
	for_each(mctbl.begin(), mctbl.end(), [&os](const MidCode& mc)->void {
			os << mc << endl; });
	return os;
}

MICROCC::CodeGen::CodeGen() {
}

MICROCC::CodeGen::~CodeGen() {
}

int MICROCC::CodeGen::genMidCode(MICROCC::ParseStack pstk,
		MidCode::OP op,
		int opcnt,
		bool result,
		MCodeTable& mctbl) {
	if (pstk.size() < opcnt) {
		cout << "*** error: not enough oprands" << endl;
		return -1;
	}
	AttrToken* poprand1(nullptr);
	AttrToken* poprand2(nullptr);
	AttrToken* presult(nullptr);
	switch (op) {
		case MidCode::OP::assign:
			mctbl.push_back({op, "a", "", "=c"});
			break;
		case MidCode::OP::add:
		case MidCode::OP::sub:
		case MidCode::OP::mul:
		case MidCode::OP::div:
		case MidCode::OP::mod:
			poprand1 = &pstk.top();
			cout << presult->addr << endl;
			pstk.pop();
			poprand2 = &pstk.top();
			pstk.pop();
			presult = &pstk.top();
			mctbl.push_back({op, "a", "b", to_string(presult->addr)});
			break;
		case MidCode::OP::push:
		case MidCode::OP::pop:
		case MidCode::OP::jmp:
			mctbl.push_back({op, "addr", "", ""});
			break;
	}
	return 0;
}

int
MICROCC::CodeGen::genObjCode(const MCodeTable& mctbl, OCodeTable& octbl) {
	return 0;
}
