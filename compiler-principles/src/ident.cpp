#include "ident.h"

using namespace std;

std::ostream&
MICROCC::operator<<(std::ostream& os, const Ident& ident) {
	os << ident.scplvl << "\t" << ident.name << "\t" << ident.val
		<< "\t" << ident.addr;
	return os;
}

std::ostream&
MICROCC::operator<<(std::ostream& os, const IdentTable& idtbl) {
	os << "Identifier Table (size: " << idtbl.size() << "):\n"
		<< "scope\tname\tvalue\taddr\n";

	for_each(idtbl.begin(), idtbl.end(), [&os](const Ident& ident) {
		os << ident << endl; });

	return os;
}

MICROCC::IdentTable::IdentTable()
: m_tmpVar(0),
m_varAddr(0)
{}

MICROCC::IdentTable::~IdentTable() {}

size_t
MICROCC::IdentTable::genAddr(IdentType idt) {
	size_t sz(0);
	size_t ret(m_varAddr);
	switch (idt) {
		case IdentType::FUNC:
		case IdentType::INT:
			sz = 4;
			break;
		case IdentType::DOUBLE:
			sz = 8;
			break;
		case IdentType::BOOL:
			sz = 2;
			break;
	}
	m_varAddr += sz;
	return ret;
}

MICROCC::Ident&
MICROCC::IdentTable::genTmp(MICROCC::IdentType idt) {
	push_back({0, idt, string("_tmp").append(to_string(m_tmpVar++)),
			"val", genAddr(idt)});
	return this->back();
}

std::list<MICROCC::Ident>::iterator
MICROCC::IdentTable::lookup(const std::string& name) {
	for (auto it = this->begin(); it != this->end(); ++it) {
		if (it->name == name)
			return it;
	}
	return this->end();
}

bool
MICROCC::IdentTable::remove(const std::string& name) {
	auto it = lookup(name);
	if (it != this->end()) {
		erase(it);
		return true;
	}
	return false;
}

