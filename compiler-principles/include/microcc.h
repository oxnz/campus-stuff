#pragma once

#include "token.h"

class Parser;
class CodeGen;
namespace MICROCC {
	class Microcc {
	public:
		Microcc();
		~Microcc();
		int compile(const char* fpath);
	private:
		char* m_sf;
		size_t m_sfsiz;
		Parser* m_pparser;
		//TokenTable m_toktbl;
	};
}
