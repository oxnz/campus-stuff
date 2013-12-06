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

list<MICROCC::Ident>::iterator 
MICROCC::IdentTable::lookup(const std::string& name) {
			for (std::list<Ident>::iterator it = this->begin();
					it != this->end(); ++it) {
				if (it->name == name)
					return it;
			}
			return this->end();
}

bool
MICROCC::IdentTable::remove(const std::string& name) {
	list<MICROCC::Ident>::iterator it = lookup(name);
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

int
MICROCC::CodeGen::genMidCode(const TokenTable& toktbl, MCodeTable& mctbl) {
	return 0;
}

int
MICROCC::CodeGen::genObjCode(const MCodeTable& mctbl, OCodeTable& octbl) {
	return 0;
}
