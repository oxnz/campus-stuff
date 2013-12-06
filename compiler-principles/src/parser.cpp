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
#include "parser.h"
#include "grammer.h"
#include "token.h"
#include "codegen.h"

using namespace std;

bool
MICROCC::Parser::parse(const TokenTable& toktbl, MCodeTable& mctbl) {
	cout << endl << "parsing" << endl;
	ParseStack pstk;
	CodeGen codegen;
	struct ReduceItem {
		MidCode::OP op;
		int opcnt;
		bool result;
	};
	
	ReduceItem ReduceTable[10] = {
		{MidCode::OP::add, 2, true},
		{MidCode::OP::sub, 2, true},
	};

	int stat = 0;
	for (TokenTable::const_iterator it = toktbl.begin();
			it != toktbl.end(); ++it) {
		cout << "(" << stat << "," << static_cast<int>(it->m_type) << endl;
		ActGoItem agit = ActGoTable[stat][static_cast<int>(it->m_type)];
		switch (agit.op) {
			case AGOP::ACCEPT:
				if (!pstk.empty()) {
					cout << "*** error: accepted before stack is empty" << endl;
					goto errorproc;
				}
				return true;
			case AGOP::SHIFT:
				cout << "shift" << endl;
				pstk.push(StackNode(*it, agit.stat));
				break;
			case AGOP::REDUCE:
				cout << "reduce" << endl;
				codegen.genMidCode(pstk, ReduceTable[agit.stat].op,
						ReduceTable[agit.stat].opcnt,
						ReduceTable[agit.stat].result, mctbl);
				break;
			case AGOP::GOTO:
				cout << "goto" << endl;
				pstk.top().stat = agit.stat;
				break;
			case AGOP::ERROR:
				cout << "*** error: Unexpected token: " << *it << endl;
				goto errorproc;
				break;
		}
		pstk.push(StackNode(*it));
		//cout << t << " ";
	}
errorproc:
	while (!pstk.empty()) {
		pstk.pop();
	}
	return false;
}
