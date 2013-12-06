#include <stdio.h>
#include <string.h>
#include <iconv.h>

int main() {
	iconv_t cd = iconv_open("GBK", "UTF-8");
	if (cd == (iconv_t)-1) {
		perror("iconv_open");
		return -1;
	}
	char pfrom[] = "你好hello";
	size_t pfsz = strlen(pfrom);
	char pto[128];
	size_t ptsz = sizeof(pto);
	printf("from: %s len: %zd to: %p len: %zd\n", pfrom, pfsz, pto, ptsz);
	char* pi = pfrom;
	char* po = pto;
	if (iconv(cd, &pi, &pfsz, &po, &ptsz) == -1) {
		perror("iconv");
		iconv_close(cd);
		return -1;
	}
	printf("from: %s len: %zd to: %p len: %zd\n", pfrom, pfsz, pto, ptsz);
	iconv_close(cd);
	printf("converted str: %s\n", pto);

	return 0;
}
