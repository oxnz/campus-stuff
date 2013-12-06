#pragma once

#include <list>
#include <iostream>

namespace MICROCC {
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

	class MCodeTable : public std::list<MidCode> {
	};
}
