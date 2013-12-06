#pragma once

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
	const int STATCNT(10);
	const int TOKTYPECNT(6);

	enum class AGOP : int {
		ACCEPT,
		SHIFT,
		REDUCE,
		GOTO,
		ERROR,
	};

	typedef int Status;
	struct AGItem {
		AGOP op;
		Status stat;
	};

	AGItem ActionGotoTable[STATCNT][TOKTYPECNT] {
		{
			{AGOP::SHIFT, 1}, {AGOP::SHIFT, 1}, {AGOP::ACCEPT, 0},
			{AGOP::GOTO, 4}, {AGOP::GOTO, 4}, {AGOP::GOTO, 5},
		},
		{
			{AGOP::REDUCE, 1}, {AGOP::ERROR, 1}, {AGOP::ERROR, 0},
			{AGOP::GOTO, 5}, {AGOP::GOTO, 3}, {AGOP::GOTO, 1},
		},
	};
}
