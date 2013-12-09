/*
 * File: product.cpp
 * Description: generate first & follow set of a grammer
 * TODO: Add LALR(1) generator
 */
#include <iostream>

#include <set>
#include <map>

class LRParser {
public:
	LRParser() {}
	~LRParser() {}
	int addproduc(const std::string& p) {
		bool init(false);
		std::string ident("");
		for (auto c : p) {
			switch (c) {
				case '<':
					init = true;
					break;
				case '>':
					if (init) {
						std::cout << "non-terminal ident: "
							<< ident << std::endl;
						//m_vtt.add(ident);
						ident.clear();
					} else
						goto errproc;
					break;
				case '\'':
					if (init) {
						if (ident.empty())
							goto errproc;
						std::cout << "terminal char: " << ident << std::endl;
						init = false;
						ident.clear();
					} else
						init = true;
					break;
				case '\"':
					if (init) {
						if (ident.empty())
							goto errproc;
						std::cout << "terminal str: " << ident << std::endl;
						ident.clear();
						init = false;
					} else
						init = true;
					break;
				default:
					ident += c;
					break;
			}
		}
		if (init)
			return 0;
errproc:
		std::cout << "*** parse production error" << std::endl;
		return -1;
	}
private:
	//int closure(set);
};

using namespace std;

int main(int argc, char* argv[]) {
	LRParser parser;
	
	for (string line(""); !cin.eof(); getline(cin, line)) {
		if (line.size()) {
			//cout << line << endl;
			parser.addproduc(line);
		} else
			cout << "blank line" << endl;
		if (cin.eof())
			break;
	}
	return 0;
}
