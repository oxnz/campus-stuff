#pragma once

#include "common.h"
#include "token.h"

#include <list>
#include <iostream>

namespace MICROCC {
	enum IdentType : uint32_t {
		FUNC = static_cast<uint32_t>(TokenType::FUNC),
		INT = static_cast<uint32_t>(TokenType::INT),
		DOUBLE = static_cast<uint32_t>(TokenType::DOUBLE),
		BOOL = static_cast<uint32_t>(TokenType::BOOL),
	};

	struct Ident : public Token {
		Ident(const Token& tok, size_t scope = 0,
				const std::string& name = "nonset",
				size_t addr = static_cast<size_t>(-1));
		Ident(size_t scope, IdentType idt, const std::string& name,
				const std::string& value, size_t addr);
		~Ident();
		size_t m_scope;
		std::string m_name;
		size_t m_addr;
		bool m_init; // initialized or not
		friend std::ostream& operator<<(std::ostream&,
				const Ident&);
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
