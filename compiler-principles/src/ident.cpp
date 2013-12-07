#include "ident.h"

using namespace std;

MICROCC::Ident::Ident(size_t scope, IdentType idt, const std::string& name,
				const std::string& value, size_t addr)
: Token(static_cast<TokenType>(idt), value),
	m_scope(scope),
	m_name(name),
	m_addr(addr)
{}


MICROCC::Ident::~Ident() {}

std::ostream&
MICROCC::operator<<(std::ostream& os, const Ident& ident) {
	os << ident.m_scope << "\t" << ident.m_name << "\t" << ident.m_value
		<< "\t" << ident.m_addr;
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

MICROCC::IdentTable::~IdentTable() {
	this->clear();
}

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
		if (it->m_name == name)
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

