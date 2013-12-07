#include "ident.h"

using namespace std;

std::ostream&
MICROCC::operator<<(std::ostream& os, const Ident& ident) {
	os << ident.scplvl << ident.name << ident.addr;
	return os;
}

std::ostream&
MICROCC::operator<<(std::ostream& os, const IdentTable& idtbl) {
	os << "Identifier Table (size: " << idtbl.size() << "):\n"
		<< "scope\tname\taddr\n";

	for_each(idtbl.begin(), idtbl.end(), [&os](const Ident& ident) {
		os << ident << endl; });

	return os;
}
