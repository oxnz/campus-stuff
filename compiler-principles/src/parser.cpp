/*
 * File: parser.cpp
 */

/*
 * <microc> ::= <prog> // accpet sign
 * <prog> ::= <func><prog> | <stmt><prog> | <func>
 * <if> ::= if <cond> <stmt> [else <stmt>] fi
 * <for> ::= for (<decl>; <cond>; <stmt>) <stmt>
 * <while> ::= while (<cond>) <stmt>
 * <block> ::= '{' <stmts> '}'
 * <do> ::= do stmt while '('<cond>')'
 * <cond> ::= <expr> {>|>=|<|<=|!=} <expr> | <expr>
 * <expr> ::= <expr>+<term> | <expr>-<term> | -<term> | <term>
 * <term> ::= <term>*<factor> | <term>/<factor> | <term>%<factor> | <factor>
 * <factor> ::= (<expr>) | <id> | <num>
 * <stmt> ::= <decl> | <assign> | <do> | <for> | <while> | <if> | <block>
 * <decl> ::= <type><assign> | <type><id> | <decl>, <id>
 * <assign> ::= <id> = {<id>|<num>|<expr>};
 * <stmts> ::= <stmts>*
 * <func> ::= <type> <id>(<args>) <block>
 * <type> ::= int | double | bool
 * <id> ::= [a-zA-Z][a-zA-Z0-9_]*
 * <arg> ::= <type><id>
 * <args> ::= <arg>(, <arg>)*
 */

#include "errproc.h"
#include "parser.h"
#include "grammer.h"
#include "token.h"
#include "codegen.h"
#include <map>

using namespace std;

namespace MICROCC {
	struct ReduceItem {
		std::function<void(ParseStack& pstk,
				CodeGen& codegen)> reduce;
	};
	ReduceItem ReduceTable[6] = {
		{[](ParseStack& pstk, CodeGen& codegen)->void { // 1
			StackNode& term = pstk.top();
			pstk.pop();
			StackNode& op = pstk.top();
			if (op.m_type != TokenType::ADD) {
				syntaxError(op,	"expected operator ADD, not found");
			}
			pstk.pop();
			StackNode& expr = pstk.top();
			pstk.pop();
			//cout << "E->E+T:" << expr << " -> " << expr << op << term << endl;
			cout << "code: expr.addr = expr.addr + term.addr" << endl;
			//cout << "=====>" << term << expr << endl;
			IdentTable& idt = codegen.identTable();
			Ident& dstexpr = idt.genTmp(IdentType::INT);
			codegen.genMidCode(MidCode::OP::add, expr, term, dstexpr);
			dstexpr.m_type = TokenType::EXPR;
			pstk.push(dstexpr);
												  }
		},
		{[](ParseStack& pstk, CodeGen& codegen)->void { // 2
			pstk.top().m_type = TokenType::EXPR;
			//cout << "E->T:"	<< expr << " -> " << term << endl;
							  }
		},
		{[](ParseStack& pstk, CodeGen& codegen)->void { // 3
			StackNode& factor = pstk.top();
			pstk.pop();
			StackNode& op = pstk.top();
			if (op.m_type != TokenType::MUL) {
				syntaxError(op, "expected operator MUL, not found");
			}
			pstk.pop();
			pstk.top().m_type = TokenType::TERM;
			StackNode& term = pstk.top();
			pstk.pop();
			IdentTable& idt = codegen.identTable();
			Ident& dstterm = idt.genTmp(IdentType::INT);
			dstterm.m_type = TokenType::TERM;
			//cout << "T->T*F:" << term << " -> " << term << op << factor << endl;
			cout << "code: term.addr = term.addr * factor.addr" << endl;
			codegen.genMidCode(MidCode::OP::mul, term, factor, dstterm);
			pstk.push(dstterm);
							  }
		},
		{[](ParseStack& pstk, CodeGen& codegen)->void { // 4
			pstk.top().m_type = TokenType::TERM;
			//cout << "T->F:"	<< term << " -> " << term << endl;
							  }
		},
		{[](ParseStack& pstk, CodeGen& codegen)->void { // 5
			StackNode& rparen = pstk.top();
			pstk.pop();
			if (rparen.m_type != TokenType::RPAREN)
				syntaxError(rparen, "expected RPAREN, not found");
			StackNode& expr = pstk.top();
			pstk.pop();
			StackNode& lparen = pstk.top();
			pstk.pop();
			if (lparen.m_type != TokenType::LPAREN)
				syntaxError(lparen, "expected LPAREN, not found");
			StackNode& factor = expr;
			/*
			cout << "F->(E):"
				<< factor << " -> " << lparen << expr << rparen << endl;
				*/
			// genMidCode(factor.addr = expr.addr);
			factor.m_type = TokenType::FACTOR;
			pstk.push(factor);
							  }
		},
		{[](ParseStack& pstk, CodeGen& codegen)->void { // 6
			StackNode& id = pstk.top();
			pstk.pop();
			//cout << "F->id:" << id << " -> " << id << endl;
			IdentTable& idt = codegen.identTable();
			auto it = idt.lookup(id.m_value);
			if (it != idt.end())
				pstk.push(*it);
			else {
				idt.push_back({id, 0, id.m_value,
						idt.genAddr(IdentType::INT)});
				pstk.push(idt.back());
			}
			pstk.top().m_type = TokenType::FACTOR;
							  }
		},
	};

	map<TokenType, size_t> TypeTable {
		{TokenType::IDENTIFIER, 0},
		{TokenType::ADD, 1},
		{TokenType::MUL, 2},
		{TokenType::LPAREN, 3},
		{TokenType::RPAREN, 4},
		{TokenType::EOF_, 5},
		{TokenType::EXPR, 6},
		{TokenType::TERM, 7},
		{TokenType::FACTOR, 8},
	};
}

bool
MICROCC::Parser::parse(TokenTable& toktbl, CodeGen& codegen) {
	ParseStack pstk;
	int stat = 0;
	while (!toktbl.empty()) {
		Token& tok = toktbl.front();
		//cout << "tok: " << tok << endl;
		ActGoItem agit = ActGoTable[stat][TypeTable[tok.m_type]];
		switch (agit.op) {
			case AGOP::A:
				cout << "accept" << endl;
				if (pstk.size() != 1) {
					syntaxError(pstk.top(),
							"accepted before reduce to begin symbol");
					goto errorproc;
				}
				return true;
			case AGOP::S:
				//cout << "S" << agit.stat << ": " << endl;
				toktbl.pop_front();
				pstk.push(StackNode(tok, agit.stat));
				stat = agit.stat;
				break;
			case AGOP::R:
				//cout << "R" << agit.stat << ": " << endl;
				/*
				toktbl.push_front(ReduceTable[agit.stat-1].reduce(pstk,
							codegen));
				stat = pstk.empty() ? 0 : pstk.top().m_stat;
				*/
				{
					ReduceTable[agit.stat-1].reduce(pstk, codegen);
					StackNode& node = pstk.top();
					pstk.pop();
					stat = pstk.empty() ? 0 : pstk.top().m_stat;
					stat = ActGoTable[stat][TypeTable[node.m_type]].stat;
					node.m_stat = stat;
					pstk.push(node);
				}
				/*
				codegen.genMidCode(pstk, ReduceTable[agit.stat].op,
						ReduceTable[agit.stat].opcnt,
						ReduceTable[agit.stat].result, mctbl);
						*/
				break;
			case AGOP::G:
				cout << "goto" << agit.stat << endl;
				// FIXME: it is wrong to cast tok to stackNode
				cerr << "*** error: unexpected GOTO" << endl;
				toktbl.pop_front();
				pstk.push(static_cast<StackNode>(tok));
				stat = agit.stat;
				pstk.top().m_stat = stat;
				break;
			case AGOP::E:
				syntaxError(tok, "unexpected token");
				goto errorproc;
				break;
		}
	}
errorproc:
	while (!pstk.empty()) {
		pstk.pop();
	}
	return false;
}
