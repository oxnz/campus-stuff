#include "codegen.h"
#include "parser.h"

using namespace std;

const char* opname[] = {
	"assign",
	"add",
	"sub",
	"mul",
	"div",
	"mod",
	"jmp",
};

ostream&
MICROCC::operator<<(ostream& os, const MICROCC::MidCode& mc) {
	os << opname[static_cast<uint8_t>(mc.op)] << "("
		<< static_cast<ssize_t>(mc.src1) << ", "
		<< static_cast<ssize_t>(mc.src2) << ") => "
		<< static_cast<ssize_t>(mc.dst);
	return os;
}

ostream&
MICROCC::operator<<(ostream& os, const MICROCC::MCodeTable& mctbl) {
	os << "MidCode Table (size: " << mctbl.size() << "):" << endl;
	for_each(mctbl.begin(), mctbl.end(), [&os](const MidCode& mc)->void {
			os << mc << endl; });
	return os;
}

MICROCC::CodeGen::CodeGen()
{}

MICROCC::CodeGen::~CodeGen() {
}

int
MICROCC::CodeGen::genMidCode(MidCode::OP op, const Ident& id1,
		const Ident& id2,
		const Ident& ret) {
	cout << "gen code" << endl;
	cout << "id1: " << id1 << "\n" << "id2: " << id2 << "\n" << "ret: " << ret
		<< "\n";
	switch (op) {
		case MidCode::OP::assign:
			break;
		case MidCode::OP::add:
		case MidCode::OP::sub:
		case MidCode::OP::mul:
		case MidCode::OP::div:
		case MidCode::OP::mod:
			m_mct.push_back({op, id1.m_addr, id2.m_addr, ret.m_addr});
			break;
		case MidCode::OP::jmp:
			m_mct.push_back({op, id1.m_addr, static_cast<size_t>(-1),
					static_cast<size_t>(-1)});
			break;
	}
	return 0;
}

void
MICROCC::CodeGen::dumpMCode(std::ostream& os) {
	os << m_mct << flush;
}

int
MICROCC::CodeGen::genObjCode(const MCodeTable& mctbl, OCodeTable& octbl) {
	return 0;
}
