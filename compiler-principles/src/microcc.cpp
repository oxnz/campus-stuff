#include "preproc.h"
#include "lexical.h"
#include "parser.h"
#include "codegen.h"
#include "semantic.h"
#include "optimize.h"
#include "target.h"
#include "errproc.h"
#include "microcc.h"

#include <fstream>

using namespace std;

MICROCC::Microcc::Microcc()
	: m_pparser(new Parser())
{
}

MICROCC::Microcc::~Microcc() {
}

int
MICROCC::Microcc::compile(const char* fpath) {
	fstream sf(fpath);
	if (!sf.is_open()) {
		cerr << "cannot open file: " << fpath << endl;
		return -1;
	}
	sf.seekg(0, ios::end);
	m_sfsiz = sf.tellg();
	m_sf = new char[++m_sfsiz];
	sf.seekg(0, ios::beg);
	sf.read(m_sf, m_sfsiz);
	sf.close();
	m_sf[m_sfsiz-1] = -1;

	TokenTable toktbl;
	if (lex(m_sf, m_sfsiz, toktbl) == -1) {
		cerr << "lex parse error" << endl;
		return -1;
	}
	cout << "Token Table[" << toktbl.size() << "]:" << endl;
	for_each (toktbl.begin(), toktbl.end(), [](const Token& t)->void {
		cout << t << " "; });
	MCodeTable mctbl;
	m_pparser->parse(toktbl, mctbl);
	cout << endl << "MidCode Table[" << mctbl.size() << "]:" << endl;
	for_each (mctbl.begin(), mctbl.end(), [](const MidCode& mc)->void {
		cout << mc << "\t"; });
	cout << endl;

	return 0;
}
