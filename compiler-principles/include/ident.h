#pragma once

#include "common.h"

#include <list>
#include <iostream>

namespace MICROCC {
	struct Ident {
		uint8_t scplvl; // scope level
		std::string name;
		size_t addr;
		friend std::ostream& operator<<(std::ostream& os,
				const Ident& ident);
	};

	class IdentTable : public std::list<Ident> {
	public:
		enum class IdentType : uint32_t {
			FUNC,
			INT,
			DOUBLE,
			BOOL
		};
		IdentTable();
		~IdentTable();
		std::list<Ident>::iterator lookup(const std::string& name);
		bool remove(const std::string& name);
		friend std::ostream& operator<<(std::ostream& os,
				const IdentTable& idtbl);
	private:
	};
}
