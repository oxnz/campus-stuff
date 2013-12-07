#pragma once

#include <iostream>
#include <list>
#include <stack>

#include "common.h"
#include "token.h"
#include "ident.h"

namespace MICROCC {
	class TokenTable;

	struct StackNode : public Ident { // attributed token with status
		StackNode(const Ident& id, int stat = 0)
			: Ident(id),
			m_stat(stat)
		{}
		int m_stat;
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
			jmp,
		} op;
		size_t src1;
		size_t src2;
		size_t dst;
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
		const IdentTable& identTable() const { return m_idt; }
		IdentTable& identTable() { return m_idt; }
		void dumpMCode(std::ostream& os);
		int genMidCode(MidCode::OP op, const Ident& id1, const Ident& id2,
				const Ident& ret);
		int genObjCode(const MCodeTable& mctbl, OCodeTable& octbl);
	private:
		IdentTable m_idt;
		MCodeTable m_mct;
		OCodeTable m_oct;
	};
}
