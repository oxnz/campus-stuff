/*
 * File: parser.cpp
 */

/*
 * <microc> ::= <program> // accpet sign
 * <program> ::= <func><program> | <stmt><program> | <main_func>
 * <if_stmt> ::= if <condition> <stmt> [else <stmt>] fi
 * <for_stmt> ::= for (<declaration>; <condition>; <stmt>) <block>
 * <while_stmt> ::= while (<condition>) <block>
 * <block> ::= '{' <stmts> '}'
 * <do_stmt> ::= do stmt while (<condition>);
 * <condition> ::= <expresstion> {>|>=|<|<=|!= <expr>}
 * <expr> ::= <expr>+<term> | <term>-<term> | -<term> | <term>
 * <term> ::= <term>*<factor> | <term>/<factor> | <term>%<factor> | <factor>
 * <factor> ::= (<expr>) | <id>
 * <stmt> ::= <decl> | <assign> | <do_stmt> | <for_stmt>
 * 				| <while_stmt> | <if_stmt> | <block>
 * <decl> ::= <type><assign> | <type><id>';'
 * <assign> ::= <id> = {<id>|<num>|<expr>};
 * <stmts> ::= <stmt>*
 * <main_func> ::= int main'('')' <block>
 * <function> ::= <type> <id>(<arg>(, <arg>)) <block>
 * <type> ::= int | double | bool
 * <id> ::= [a-zA-Z][a-zA-Z0-9_]*
 * <arg> ::= <type><id>
 * <args> ::= <arg>'(', <arg>')'*
 */
#include "errproc.h"
#include "parser.h"
#include "grammer.h"
#include "token.h"
#include "codegen.h"

using namespace std;

namespace MICROCC {
	struct ReduceItem {
		std::function<StackNode&(ParseStack& pstk)> reduce;
	};
	ReduceItem ReduceTable[6] = {
		{[](ParseStack& pstk)->StackNode& { // 1
			StackNode& term = pstk.top();
			pstk.pop();
			StackNode& op = pstk.top();
			pstk.pop();
			StackNode& expr = pstk.top();
			pstk.pop();
			//cout << "E->E+T:" << expr << " -> " << expr << op << term << endl;
			cout << "code: expr.addr = expr.addr + term.addr" << endl;
			// genMidCode(expr.addr = expr.addr + term.addr)
			expr.m_type = TokenType::EXPR;
			return expr;
												  }
		},
		{[](ParseStack& pstk)->StackNode& { // 2
			StackNode& term = pstk.top();
			pstk.pop();
			StackNode& expr = term;
			expr.m_type = TokenType::EXPR;
			//cout << "E->T:"	<< expr << " -> " << term << endl;
			return expr;
							  }
		},
		{[](ParseStack& pstk)->StackNode& { // 3
			StackNode& factor = pstk.top();
			pstk.pop();
			StackNode& op = pstk.top();
			pstk.pop();
			StackNode& term = pstk.top();
			pstk.pop();
			//cout << "T->T*F:" << term << " -> " << term << op << factor << endl;
			cout << "code: term.addr = term.addr * factor.addr" << endl;
			// genMidCode(term.addr = term.addr * factor.addr)
			term.m_type = TokenType::TERM;
			return term;
							  }
		},
		{[](ParseStack& pstk)->StackNode& { // 4
			StackNode& term = pstk.top();
			pstk.pop();
			//cout << "T->F:"	<< term << " -> " << term << endl;
			term.m_type = TokenType::TERM;
			return term;
							  }
		},
		{[](ParseStack& pstk)->StackNode& { // 5
			StackNode& rparen = pstk.top();
			pstk.pop();
			StackNode& expr = pstk.top();
			pstk.pop();
			StackNode& lparen = pstk.top();
			pstk.pop();
			StackNode& factor = expr;
			/*
			cout << "F->(E):"
				<< factor << " -> " << lparen << expr << rparen << endl;
				*/
			// genMidCode(factor.addr = expr.addr);
			factor.m_type = TokenType::FACTOR;
			return factor;
							  }
		},
		{[](ParseStack& pstk)->StackNode& { // 6
			StackNode& id = pstk.top();
			pstk.pop();
			//cout << "F->id:" << id << " -> " << id << endl;
			StackNode& factor = id;
			factor.m_type = TokenType::FACTOR;
			return factor;
							  }
		},
	};
}

bool
MICROCC::Parser::parse(TokenTable& toktbl, MCodeTable& mctbl) {
	cout << endl << "parsing" << endl;
	ParseStack pstk;
	CodeGen codegen;
	int stat = 0;
	int ttyp(0); // token type
	while (!toktbl.empty()) {
		Token& tok = toktbl.front();
		//cout << "tok: " << tok << endl;
		switch (tok.m_type) {
			case TokenType::IDENTIFIER:
				ttyp = 0;
				break;
			case TokenType::ADD:
				ttyp = 1;
				break;
			case TokenType::MUL:
				ttyp = 2;
				break;
			case TokenType::LPAREN:
				ttyp = 3;
				break;
			case TokenType::RPAREN:
				ttyp = 4;
				break;
			case TokenType::EOF_:
				ttyp = 5;
				break;
			case TokenType::EXPR:
				ttyp = 6;
				break;
			case TokenType::TERM:
				ttyp = 7;
				break;
			case TokenType::FACTOR:
				ttyp = 8;
				break;
			default:
				syntaxError(tok, "unexpected token");
				break;
		}
		ActGoItem agit = ActGoTable[stat][ttyp];
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
				toktbl.push_front(ReduceTable[agit.stat-1].reduce(pstk));
				stat = pstk.empty() ? 0 : pstk.top().stat;
				/*
				codegen.genMidCode(pstk, ReduceTable[agit.stat].op,
						ReduceTable[agit.stat].opcnt,
						ReduceTable[agit.stat].result, mctbl);
						*/
				break;
			case AGOP::G:
				//cout << "goto" << agit.stat << endl;
				toktbl.pop_front();
				pstk.push(static_cast<StackNode>(tok));
				stat = agit.stat;
				pstk.top().stat = stat;
				break;
			case AGOP::E:
				syntaxError(tok, "unexpected token");
				goto errorproc;
				break;
		}
		//pstk.push(StackNode(*it));
		//cout << t << " ";
	}
errorproc:
	while (!pstk.empty()) {
		pstk.pop();
	}
	return false;
}
