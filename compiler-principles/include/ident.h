#pragma once

#include <list>
#include <iostream>

namespace MICROCC {
	struct IDENT {
		uint8_t scopelvl;
		std::string name;
		size_t addr;
	};
	class IdentTable : public std::list<IDENT> {
	};
}
