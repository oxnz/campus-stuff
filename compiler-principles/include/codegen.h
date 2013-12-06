#pragma once

#include <iostream>
#include <list>

namespace MICROCC {
	class TokenTable;
	struct Ident {
		uint8_t scopelvl;
		std::string name;
		size_t addr;
	};

	class IdentTable : public std::list<Ident> {
	};

	struct MidCode {
		enum class OP : uint8_t {
			add,
			sub,
			mul,
			div,
			mod,
			push,
			pop,
			jmp,
		} op;
		std::string operand1;
		std::string operand2;
		std::string result;
		friend std::ostream& operator<<(std::ostream& os, const MidCode& mc);
	};

	struct ObjCode {
	};

	class MCodeTable : public std::list<MidCode> {
	};

	class OCodeTable : public std::list<ObjCode> {
	};

	class CodeGen {
	public:
		CodeGen();
		~CodeGen();
		int genMidCode(const TokenTable& toktbl, MCodeTable& mctbl);
		int genObjCode(const MCodeTable& mctbl, OCodeTable& octbl);
	private:
	};
}
