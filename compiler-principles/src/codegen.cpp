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

list<MICROCC::Ident>::iterator 
MICROCC::IdentTable::lookup(const std::string& name) {
			for (IdentTable::iterator it = this->begin();
					it != this->end(); ++it) {
				if (it->name == name)
					return it;
			}
			return this->end();
}

bool
MICROCC::IdentTable::remove(const std::string& name) {
	IdentTable::iterator it = lookup(name);
	if (it != this->end()) {
		erase(it);
		return true;
	} else
		return false;
}

ostream&
MICROCC::operator<<(ostream& os, const MICROCC::MidCode& mc) {
	os << opname[static_cast<uint8_t>(mc.op)] << "("
		<< mc.operand1 << ", " << mc.operand2 << ") => " << mc.result;
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
	cout << "gen..." << endl;
	mctbl.push_back({op, "a", "b", "c"});
	return 0;
}

int
MICROCC::CodeGen::genObjCode(const MCodeTable& mctbl, OCodeTable& octbl) {
	return 0;
}
