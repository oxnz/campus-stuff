#include "token.h"
#include <map>

using namespace std;
using namespace MICROCC;

Token::Token(TokenType type, const TokenValue& value, const Position& pos)
	: m_type(type),
	m_value(value),
	m_pos(pos)
{
}

namespace MICROCC {
	map<TokenType, const string> ttname = {
		{TokenType::BOOL, "BOOL"},
		{TokenType::INT, "INT"},
		{TokenType::DOUBLE, "DBLE"},
		{TokenType::CHAR, "CHAR"},
		{TokenType::STR, "STR"},
		{TokenType::TRUE, "TRUE"},
		{TokenType::FALSE, "FALSE"},

		{TokenType::INTVAL, "(INT)"},
		{TokenType::DOUBLEVAL, "(DBLE)"},
		{TokenType::CHARVAL, "(CHAR)"},
		{TokenType::STRVAL, "(STR)"},

		{TokenType::IF, "IF"},
		{TokenType::ELIF, "ELIF"},
		{TokenType::ELSE, "ELSE"},
		{TokenType::FI, "FI"},

		{TokenType::CASE, "CASE"},
		{TokenType::IN, "IN"},
		{TokenType::ESAC, "ESAC"},

		{TokenType::IDENTIFIER, "ID"},

		{TokenType::ASSIGN, "ASSIGN"},
		{TokenType::ADD, "ADD"},
		{TokenType::SUB, "SUB"},
		{TokenType::MUL, "MUL"},
		{TokenType::DIV, "DIV"},
		{TokenType::LT, "LT"},
		{TokenType::LE, "LE"},
		{TokenType::NE, "NE"},
		{TokenType::EQ, "EQ"},
		{TokenType::GT, "GT"},
		{TokenType::GE, "GE"},
		{TokenType::NOT, "NOT"},

		{TokenType::COLON, "COLON"},
		{TokenType::AMP, "AMP"},
		{TokenType::CARET, "CARET"},
		{TokenType::UNDERSCORE, "UNDERSCORE"},
		{TokenType::BACKSLASH, "BLACKSLASH"},
		{TokenType::DOT, "DOT"},
		{TokenType::PIPE, "PIPE"},
		{TokenType::POUND, "POUND"},

		{TokenType::LSUBSCRIPT, "LSUBSPT"},
		{TokenType::RSUBSCRIPT, "RSUBSPT"},

		{TokenType::LPAREN, "LPAREN"},
		{TokenType::RPAREN, "RPAREN"},
		{TokenType::LBRACKET, "LBRACKT"},
		{TokenType::RBRACKET, "RBRACKT"},
		{TokenType::QUOTE, "QUOTE"},
		{TokenType::TILDE, "TILDE"},

		{TokenType::COMMA, "COMMA"},
		{TokenType::SEMICOLON, "SEMICLN"},
		{TokenType::SPACE, "SPACE"},
		{TokenType::TAB, "TAB"},
		{TokenType::EOL, "EOF"},
		{TokenType::EOF_, "EOF"},
		{TokenType::BEGIN, "BEGIN"},
		{TokenType::END, "END"},

		{TokenType::FUNC, "FUNC"},

		{TokenType::EXPR, "EXPR"},
		{TokenType::TERM, "TERM"},
		{TokenType::FACTOR, "FACTOR"},
	};
}

ostream&
MICROCC::operator<<(ostream& os, const Position& pos) {
	os << "(" << static_cast<ssize_t>(pos.row == -1 ? -1 : pos.row)
		<< ", "
		<< static_cast<ssize_t>(pos.col == -1 ? -1 : pos.col) << ")";
	return os;
}

ostream&
MICROCC::operator<<(ostream& os, const Token& t) {
	os << t.m_pos << "  \t" << ttname[t.m_type] << "\t";
	switch (t.m_type) {
		case TokenType::POUND:
		case TokenType::INTVAL:
		case TokenType::DOUBLEVAL:
		case TokenType::CHARVAL:
		case TokenType::IDENTIFIER:
			os << t.m_value;
			break;
		case TokenType::STRVAL:
			os << "\"" << t.m_value << "\"";
			break;
		default:
			os << "nonset";
			break;
	}
	return os;
}

TokenTable::TokenTable() {}

TokenTable::~TokenTable() {
	this->clear();
}

std::ostream&
MICROCC::operator<<(std::ostream& os,
		const TokenTable& toktbl) {
	os << "====> Token Table (size: " << toktbl.size()
		<< "):\n-------------------------------------------\n"
		<< "position\ttype\tvalue\n";
	for_each (toktbl.begin(), toktbl.end(), [&os](const Token& t)->void {
		os << t << endl; });
	os << "===========================================\n";
	return os;
}
