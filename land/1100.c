#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void inline readx(n) {
	while(n--)
		getchar();
}

int main(void) {
	char *s[8] = {
		"littleken",
		"knuthocean",
		"dongfangxu",
		"zap",
		"kittig",
		"robertcui",
		"forest",
		"flirly"
	};
	int a[8] = {0, 0, 0, 0, 0, 0, 0, 0};
	char first[3] = "\0\0\0";
	int T;
	scanf("%d\n", &T);
	char c;
	while (T--) {
		while ((c = getchar()) != '\n') {
			switch (c) {
				case 'l':
					a[0]++;
					readx(8);
					puts("little\n");
					break;
				case 'k':
					if ((c = getchar()) == 'n') {
						a[1]++;
						readx(8);
					}
					else {
						a[4]++;
						readx(5);
					}
					break;
				case 'd':
					a[2]++;
					readx(9);
					break;
				case 'z':
					a[3]++;
					readx(2);
					break;
				case 'r':
					a[5]++;
					readx(8);
					break;
				case 'f':
					if ((c = getchar()) == 'o') {
						a[6]++;
						readx(4);
					}
					else {
						a[7]++;
						readx(5);
					}
					break;
			}
		}
	}

	return 0;
}
