#include "microcc.h"

int help(int ecode) {
	(ecode == 0 ? std::cout : std::cerr) <<
		"Usage:" << std::endl <<
		"microc a small c compiler" << std::endl <<
		"microc [options] <sourcefile>" <<
		"  Options:" <<
		std::endl;

	return ecode;
}

using MICROCC::Microcc;

int main(int argc, char* argv[]) {
	if (argc == 1)
		return help(0);
	Microcc cc;
	cc.compile(argv[1]);

	return 0;
}
