#pragma once

/*
 * Vn = {S, E, T, F}
 * Vt = {var, id, =, input, (, ), output, +, *, num, $}
 * AGTable:
 * --------------------------------------------------------------------------
 * status| 				ACTION							     |     GOTO      
 * --------------------------------------------------------------------------
 *       | var | id | = | in | ( | ) | out | + | * | num | $ | S | E | T | F
 * --------------------------------------------------------------------------
 *   0   | s2  | s3 |   | s4 |   |   | s5  |   |   |     |   | 1 |   |   |  
 *   1   |     |    |   |    |   |   |     |   |   |     | A |   |   |   |  
 *   2   |     | r1 |   |    |   |   |     |   |   |     |   |   |   |   |  
 *   3   |     |    | s7|    |   |   |     |   |   |     |   |   |   |   |  
 *   4   |     |    |   |    | s8|   |     |   |   |     |   |   |   |   |  
 *   5   |     |    |   |    |   | s9|     |   |   |     |   |   |   |   |  
 *   6   |     |    |   |    |   |   |     |   |   |     |   |   |   |   |  
 *   7   |     | s14|   |    |s13|   |     |   |   | s15 |   |   |10 |11 |12
 *   8   |     | s16|   |    |   |   |     |   |   |     |   |   |   |   |  
 *   9   |     | s17|   |    |s13|   |     |   |   |     |   |   |18 |11 |  
 *   10  |     |    |   |    |   |   |     |s19|   |     |   |   |   |   |  
 *   11  |     |    |   |    |   |   |     |   |s20|     |   |   |   |   |  
 *   12  |     |    |   |    |   |   |     |   |   |     |   |   |   |   |  
 *   13  |     | s14|   |    |s13|   |     |   |   | s15 |   |   |21 |11 |  
 *   14  |     |    |   |    |   |   |     |   |   |     |   |   |   |   |  
 *   15  |     |    |   |    |   |   |     |   |   |     |   |   |   |   |  
 *   16  |     |    |   |    |   |s22|     |   |   |     |   |   |   |   |  
 *   17  |     |    |   |    |   |s23|     |   |   |     |   |   |   |   |  
 *   18  |     |    |   |    |   |s24|     |s19|   |     |   |   |   |   |  
 *   19  |     | s14|   |    |s13|   |     |   |   | s15 |   |   |   |25 |12
 *   20  |     | s14|   |    |s13|   |     |   |   | s15 |   |   |   |   |26
 *   21  |     |    |   |    |   |s27|     |s19|s20|     |   |   |   |   |  
 *   22  |     |    |   |    |   |   |     |   |   |     |   |   |   |   |  
 *   23  |     |    |   |    |   |   |     |   |   |     |   |   |   |   |  
 *   24  |     |    |   |    |   |   |     |   |   |     |   |   |   |   |  
 *   25  |     |    |   |    |   |   |     |   |   |     |   |   |   |   |  
 *   26  |     |    |   |    |   |   |     |   |   |     |   |   |   |   |  
 *   27  |     |    |   |    |   |   |     |   |   |     |   |   |   |   |  
 * --------------------------------------------------------------------------
 * G[S`]:
 * S` ::= S
 * 1. S ::= var <id>
 * 2. S ::= <id> = E
 * 3. S ::= input(<id>)
 * 4. S ::= output(<id>)
 * 5. S ::= output(E)
 * 6. E ::= E + T
 * 7. E ::= T
 * 8. T ::= T * F
 * 9. T ::= F
 * 10. F ::= (E)
 * 11. F ::= <id>
 * 12. F ::= <num>
 *
 * I0:
 * S` ::= . S
 * S ::= . var <id>
 * S ::= . <id> = E
 * S ::= . input(<id>)
 * S ::= . output(<id>)
 * S ::= . output(E)
 *
 * I1(I0 + S):
 * S` ::= S .
 *
 * I2(I0 + var):
 * S ::= var . <id>
 *
 * I3(I0 + <id>):
 * S ::= <id> . = E
 *
 * I4(I0 + input):
 * S ::= input . (<id>)
 *
 * I5(I0 + output):
 * S ::= output . (<id>)
 * S ::= output . (E)
 *
 * I6(I2 + <id>):
 * S ::= var <id> .
 *
 * I7(I3 + =):
 * S ::= <id> = . E
 * E ::= . E + T
 * E ::= . T
 * T ::= . T * F
 * T ::= . F
 * F ::= . (E)
 * F ::= . <id>
 * F ::= . <num>
 *
 *
 * I8(I4 + '('):
 * S ::= input( . <id>)
 *
 * I9(I5 + '('):
 * S ::= output( . <id>)
 * S ::= output( . E)
 * E ::= . E + T
 * E ::= . T
 * T ::= . T * F
 * T ::= . F
 * F ::= . (E)
 * F ::= . <id>
 * F ::= . <num>
 *
 * I10(I7 + E):
 * S ::= <id> = E .
 * E ::= E . + T
 *
 * I11(I7 + T):
 * E ::= T .
 * T ::= T . * F
 *
 * I12(I7 + F):
 * T ::= F .
 * 
 * I13(I7 + '('):
 * F ::= ( . E)
 * E ::= . E + T
 * E ::= . T
 * T ::= . T * F
 * T ::= . F
 * F ::= . (E)
 * F ::= . <id>
 * F ::= . <num>
 *
 * I14(I7 + <id>):
 * F ::= <id> .
 * I15(I7 + <num>):
 * F ::= <num> .
 *
 * I16(I8 + <id>):
 * S ::= input(<id> . )
 * I17(I9 + <id>):
 * S ::= output(<id> . )
 * F ::= <id> .
 * I18(I9 + E):
 * S ::= output(E . )
 * E ::= E . + T
 * (I9 + T) = I11
 * (I9 + '(') = I13
 * (I9 + <num>) = I15
 * I19(I10 + '+'):
 * E ::= E + . T
 * T ::= . T * F
 * T ::= . F
 * F ::= . (E)
 * F ::= . <id>
 * F ::= . <num>
 * I20(I11 + '*'):
 * T ::= T * . F
 * F ::= . (E)
 * F ::= . <id>
 * F ::= . <num>
 * I21(I13 + E):
 * F ::= (E . )
 * E ::= E . + T
 * (I13 + T) = I11
 * (I13 + '(') = I13
 * (I13 + <id>) = I14
 * (I13 + <num>) = I15
 * I22(I16 + ')'):
 * S ::= input(<id>) .
 * I23(I17 + ')'):
 * S ::= output(<id>) .
 * I24(I18 + ')'):
 * S ::= output(E) .
 * (I18 + '+') = I19
 * I25(I19 + T):
 * E ::= E + T .
 * T ::= T . * F
 * (I19 + F) = I12
 * (I19 + '(') = I13
 * (I19 + <id>) = I14
 * (I19 + <num>) = I15
 * I26(I20 + F):
 * T ::= T * F .
 * (I20 + '(') = I13
 * (I20 + <id>) = I14
 * (I20 + <num>) = I15
 * I27(I21 + ')'):
 * F ::= (E) .
 * (I21 + '+') = I19
 * (I25 + '*') = I20
 */

/*
 * G[<microc>]:
 * <microc> ::= <prog> // accpet sign
 * <prog> ::= <func><prog> | <stmt><prog> | <func>
 * <if> ::= if <cond> <stmt> [else <stmt>] fi
 * <for> ::= for '('(<decl> | <assign>)';' <cond>';' <stmt>')' <stmt>
 * <while> ::= while (<cond>) <stmt>
 * <block> ::= '{' <stmts> '}'
 * <do> ::= do stmt while '('<cond>')'';'
 * <cond> ::= <expr> {>|>=|<|<=|!=} <expr> | <expr>
 * <expr> ::= <expr>+<term> | <expr>-<term> | -<term> | <term>
 * <term> ::= <term>*<factor> | <term>/<factor> | <term>%<factor> | <factor>
 * <factor> ::= (<expr>) | <id> | <num>
 * <stmt> ::= <decl>';' | <assign>';' | <do> | <for> | <while> | <if>
 * 			  | <block>
 * <decl> ::= <type><assign> | <type><id> | <decl>, <id>
 * <assign> ::= <id> = {<id>|<num>|<expr>}
 * <stmts> ::= <stmts>+
 * <func> ::= <type> <id>(<args>) <block>
 * <type> ::= int | double | bool
 * <id> ::= [a-zA-Z][a-zA-Z0-9_]*
 * <arg> ::= <type><id>
 * <args> ::= <arg>(, <arg>)*
 */

// TODO: Add input && output stmt

#include "token.h"
#include "common.h"

namespace MICROCC {
	/*
	 *               ACTION-GOTO TABLE
	 * --------------------------------------------------;
	 * STATUS|	ACTION   	| 		GOTO 	         	 |
	 * --------------------------------------------------;
	 *  	 | <id> | ( | ) | <expr> | <term> | <factor> |
	 * --------------------------------------------------;
	 *   0	 |  s1  | s1| A |   4    |   4    |    5     |
	 * --------------------------------------------------;
	 *   1   |  r1  |   |   |   5    |   3    |    1     |
	 * --------------------------------------------------;
	 */
	const int STATCNT(12); // statuc count
	const int TOKTYPCNT(9); // token type count

	enum class AGOP : int {
		A, // accept
		S, // shift
		R, // reduce
		G, // goto
		E, // error
	};

	typedef int Status;
	struct ActGoItem {
		AGOP op;
		Status stat;
	};

	ActGoItem ActGoTable[STATCNT][TOKTYPCNT] {
		{ // 0
			{AGOP::S, 5}, {AGOP::E, 0}, {AGOP::E, 0},
			{AGOP::S, 4}, {AGOP::E, 0}, {AGOP::E, 0},
			{AGOP::G, 1}, {AGOP::G, 2}, {AGOP::G, 3},
		},
		{ // 1
			{AGOP::E, 0}, {AGOP::S, 6}, {AGOP::E, 0},
			{AGOP::E, 0}, {AGOP::E, 0}, {AGOP::A, 0},
			{AGOP::E, 0}, {AGOP::E, 0}, {AGOP::E, 0},
		},
		{ // 2
			{AGOP::E, 0}, {AGOP::R, 2}, {AGOP::S, 7},
			{AGOP::E, 0}, {AGOP::R, 2}, {AGOP::R, 2},
			{AGOP::E, 0}, {AGOP::E, 0}, {AGOP::E, 0},
		},
		{ // 3
			{AGOP::E, 0}, {AGOP::R, 4}, {AGOP::R, 4},
			{AGOP::E, 0}, {AGOP::R, 4}, {AGOP::R, 4},
			{AGOP::E, 0}, {AGOP::E, 0}, {AGOP::E, 0},
		},
		{ // 4
			{AGOP::S, 5}, {AGOP::E, 0}, {AGOP::E, 0},
			{AGOP::S, 4}, {AGOP::E, 0}, {AGOP::E, 0},
			{AGOP::G, 8}, {AGOP::G, 2}, {AGOP::G, 3},
		},
		{ // 5
			{AGOP::E, 0}, {AGOP::R, 6}, {AGOP::R, 6},
			{AGOP::E, 0}, {AGOP::R, 6}, {AGOP::R, 6},
			{AGOP::E, 0}, {AGOP::E, 0}, {AGOP::E, 0},
		},
		{ // 6
			{AGOP::S, 5}, {AGOP::R, 4}, {AGOP::R, 4},
			{AGOP::S, 4}, {AGOP::R, 4}, {AGOP::R, 4},
			{AGOP::E, 0}, {AGOP::G, 9}, {AGOP::G, 3},
		},
		{ // 7
			{AGOP::S, 5}, {AGOP::E, 0}, {AGOP::E, 0},
			{AGOP::S, 4}, {AGOP::E, 0}, {AGOP::E, 0},
			{AGOP::E, 0}, {AGOP::E, 0}, {AGOP::G, 10},
		},
		{ // 8
			{AGOP::E, 0}, {AGOP::S, 6}, {AGOP::E, 0},
			{AGOP::E, 0}, {AGOP::S, 11}, {AGOP::E, 0},
			{AGOP::E, 0}, {AGOP::E, 0}, {AGOP::E, 0},
		},
		{ // 9
			{AGOP::E, 0}, {AGOP::R, 1}, {AGOP::S, 7},
			{AGOP::E, 0}, {AGOP::R, 1}, {AGOP::R, 1},
			{AGOP::E, 0}, {AGOP::E, 0}, {AGOP::E, 0},
		},
		{ // 10
			{AGOP::E, 0}, {AGOP::R, 3}, {AGOP::R, 3},
			{AGOP::E, 0}, {AGOP::R, 3}, {AGOP::R, 3},
			{AGOP::E, 0}, {AGOP::E, 0}, {AGOP::E, 0},
		},
		{ // 11
			{AGOP::E, 0}, {AGOP::R, 5}, {AGOP::R, 5},
			{AGOP::E, 0}, {AGOP::R, 5}, {AGOP::R, 5},
			{AGOP::E, 0}, {AGOP::E, 0}, {AGOP::E, 0},
		},
	};
}
