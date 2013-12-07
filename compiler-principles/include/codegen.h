#pragma once

#include <iostream>
#include <list>
#include <stack>

#include "common.h"
#include "token.h"
#include "ident.h"

namespace MICROCC {
	class TokenTable;

	struct AttrToken : public Token {
		AttrToken(const Token& t, int address = 0)
			: Token(t),
			addr(address)
		{}
		int addr;
	};

	struct StackNode : public AttrToken { // attributed token with status
		StackNode(const AttrToken& atok, int status = 0)
			: AttrToken(atok),
			stat(status)
		{}
		int stat;
	};

	class ParseStack : public std::stack<StackNode> {
	public:
	private:
	};

	struct MidCode {
		enum class OP : uint8_t {
			assign,
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
	public:
		friend std::ostream& operator<<(std::ostream& os,
				const MCodeTable& mctbl);
	};

	class OCodeTable : public std::list<ObjCode> {
	};

	class CodeGen {
	public:
		CodeGen();
		~CodeGen();
		int genMidCode(ParseStack pstk,
				MidCode::OP op,
				int opcnt,
				bool result,
			   	MCodeTable& mctbl);
		int genObjCode(const MCodeTable& mctbl, OCodeTable& octbl);
	private:
	};
}
