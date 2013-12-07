#include "microcc.h"

int help(int ecode) {
	(ecode == 0 ? std::cout : std::cerr) <<
		"Overview: " << "microc compiler\n" <<
		"Usage" << "microc [options] <inputs>\n" <<
		"  Options:\n" <<
		"    -h   --help     show this message and exit\n" <<
		"    -v   --version  show version info and exit" <<
		std::endl;

	return ecode;
}

const std::string VERSION("0.12");
const std::string MAJOR("0.1");
const std::string MINOR("2");
const std::string PLATFORM("x86_64-apple-darwin13.0");
const std::string COPYING("Copyright (C) 2013 Oxnz, All rights reserved.");
const std::string EMAIL("yunxinyi@gmail.com");

void version(bool verbose) {
	std::cout << "microcc version " << VERSION << " " <<
		"(" << MAJOR << MINOR << ")" <<
		"\nTarget: " << PLATFORM <<
		"\n" << COPYING <<
		//"\nBug Report: " << EMAIL <<
		std::endl;
}

int main(int argc, const char* argv[]) {
	if (argc == 1 || std::string(argv[1]) == "-h" ||
			std::string(argv[1]) == "--help")
		return help(0);
	else if (argc == 2 && (std::string(argv[1]) == "-v" ||
			std::string(argv[1]) == "--version")) {
		version(true);
		return 0;
	} else {
		MICROCC::Microcc cc;
		cc.compile(argv[1]);
	}

	return 0;
}
