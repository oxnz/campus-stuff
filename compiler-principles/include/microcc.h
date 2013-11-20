#pragma once

#include "token.h"

namespace MICROCC {
	class Microcc {
	public:
		Microcc();
		~Microcc();
		int compile(const char* fpath);
	private:
		char* m_sf;
		size_t m_sfsiz;
		//TokenTable m_toktbl;
	};
}
