#pragma once

#include "token.h"
#include "common.h"

namespace MICROCC {
	class Parser;
	class CodeGen;
	class Microcc {
	public:
		Microcc();
		~Microcc();
		int compile(const char* fpath);
	private:
		char* m_psf; // -> source file cache
		size_t m_sfsiz; // source file cache length
		Parser* m_pparser; // -> parser
	};
}
