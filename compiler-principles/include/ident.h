#pragma once

#include "common.h"

#include <list>
#include <iostream>

namespace MICROCC {
	enum class IdentType : uint32_t {
		FUNC,
		INT,
		DOUBLE,
		BOOL
	};

	struct Ident {
		size_t scplvl; // scope level
		IdentType idt;
		std::string name;
		std::string val;
		size_t addr;
		friend std::ostream& operator<<(std::ostream& os,
				const Ident& ident);
	};

	class IdentTable : public std::list<Ident> {
	public:
		IdentTable();
		~IdentTable();
		size_t genAddr(IdentType idt);
		Ident& genTmp(IdentType idt);
		std::list<Ident>::iterator lookup(const std::string& name);
		bool remove(const std::string& name);
		friend std::ostream& operator<<(std::ostream& os,
				const IdentTable& idtbl);
	private:
		size_t m_tmpVar;
		size_t m_varAddr;
	};
}
