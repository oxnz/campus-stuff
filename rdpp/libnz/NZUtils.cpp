#include "NZUtils.h"

#include <cstddef>
#include <cctype>
#include <iomanip>

using namespace std;

void NZUtils::NZHexDump(const uint8_t* p,
                        size_t len,
                        size_t col,
                        size_t gsz,
                        bool capital) {
	uint8_t hextbl[17] = "0123456789ABCDEF";
	if (!capital)
		for (size_t i = 10; i < 17; ++i)
			hextbl[i] -= 'A' - 'a';
	size_t hexlen = col*2 + (col-gsz+1)/gsz;
	size_t buflen = hexlen + 2 + col + 3;
	uint8_t* buf = new uint8_t[buflen];
	buf[hexlen] = ' ';
    buf[hexlen+1] = '|';
    buf[buflen-3] = '|';
	buf[buflen-2] = '\n';
	buf[buflen-1] = '\0';
	for (size_t i = 0; i < len; i += col) {
		uint8_t* pb = buf-1;
		for (size_t j = 0; j < col;) {
			for (size_t k = 0; j < col && k < gsz; ++j, ++k) {
				*++pb = (i+j < len) ? hextbl[(p[i+j] & 0xF0) >> 4] : ' ';
				*++pb = (i+j < len) ? hextbl[p[i+j] & 0x0F] : ' ';
				buf[hexlen+j+2] = (i+j < len) ?
					(isprint(p[i+j]) ? p[i+j] : '.') : ' ';
			}
			*++pb = ' ';
		}
		char fc = cout.fill('0');
		cout << setw(8) << hex << setfill('0') << i << ": "
			<< buf << setfill(fc) << dec;
	}
	delete[] buf;
}
