/*
 * File		:	des.c
 * Description	:	A simple DES demo program to show how DES works
 * Author	:	yunxinyi@gmail.com
 * Updated	:	Sat Mar 23 21:10:41 CST 2013
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "des.h"

#ifdef DEBUG
void pbin(u8* s, int x) {
	int i;
	for (i = 0; i < x; ++i) {
		if (i % 8 == 0)
			putchar(' ');
		printf("%d", s[i]);
	}
}
#endif

void genKey(u8 key[]) {
	// spend another 56 bytes to avoid left shift
	u8 C0[56];
	u8 D0[56];
	u8* ci = C0;
	u8* di = D0;
	int i;
	int j;

	// Init C0 & D0
	for (i = 0; i < 28; ++i)
		C0[i] = (key[(P1[i]-1)>>3] >> (7 - (P1[i]-1)%8)) & 0x01;
	for (i = 0; i < 28; ++i)
		D0[i] = (key[(P1[28+i])>>3] >> (7 - (P1[28+i]-1)%8)) & 0x01;
	for (i = 0; i < 28; ++i) {
		ci[28+i] = ci[i];
		di[28+i] = di[i];
	}
	// init KEY
	for (j = 0; j < 16; ++j) {
		// ci & di do left shift(fake shift)
		ci += KTL[j];
		di += KTL[j];
		for (i = 0; i < 48; ++i) {
			if (P2[i] > 28)
				KEY[j][i] = di[P2[i]-29];
			else
				KEY[j][i] = ci[P2[i]-1];
		}
#ifdef DEBUG
		printf("key %02d: ", j+1);
		pbin(KEY[j], 48);
		putchar('\n');
#endif
	}
}

/*
 * ret = f(Ri_1, Ki)
 */
void f(u8 Ri_1[], u8 Ki[], u8 ret[]) {
	int i;
	int j;
	u8 tmp[48];
	u8 s[48];
	u8 _t;

	for (i = 0; i < 48; ++i) {
		tmp[i] = Ri_1[E[i]-1];
		tmp[i] ^= Ki[i];
	}
	for (i = 0; i < 8; ++i) {
		// S-Box
		_t = S[i][tmp[i*6]*2 + tmp[i*6+5]][tmp[i*6+1]*8 +
			tmp[i*6+2]*4 + tmp[i*6+3] * 2 + tmp[i*6+4]];
		// 6 to 4
		s[i*4] = (_t >> 3) & 0x01;
		s[i*4+1] = (_t >> 2) & 0x01;
		s[i*4+2] = (_t >> 1) & 0x01;
		s[i*4+3] = (_t >> 0) & 0x01;
	}
	// P
	for (i = 0; i < 32; ++i) {
		ret[i] = s[P[i]-1];
	}
}

void encMsg(u8 m[], u8 c[], u8 d) {
	u8 msg[64];
	u8 fret[32];
	u8* L[17] = { msg, };
	u8* R[17] = {msg + 32, };
	int i;
	int j;

	for (i = 0; i < 64; ++i)
		msg[i] = m[IP[i]-1];
	for (j = 0; j < 16; ++j) {
		L[j+1] = R[j];
		if (d)	// decryption, reverse key order
			f(R[j], KEY[15-j], fret);
		else	// encryption
			f(R[j], KEY[j], fret);
		R[j+1] = L[j];
		for (i = 0; i < 32; ++i) {
			R[j+1][i] ^= fret[i];
		}
		printf("L%02d: ", j+1);
		pbin(L[j+1], 32);
		printf("\nR%02d: ", j+1);
		pbin(R[j+1], 32);
		printf("\n");
	}
	// swap(Li, Ri)
	for(i = 0; i < 32; ++i) {
		msg[i+32] ^= msg[i];
		msg[i] ^= msg[i+32];
		msg[i+32] ^= msg[i];
	}
	for (i = 0; i < 64; ++i) {
		c[i] = msg[IP_1[i]-1];
	}
}

void des(u8 k[], u8 m[]) {
	u8 msg[64];
	u8 c[64];
	int i;
	genKey(k);
	for (i = 0; i < 64; ++i) {
		msg[i] = (m[i/8] >> (7 - i%8)) & 0x01;
	}
	printf("\nM: ");
	pbin(msg, 64);
	printf("\n");
	encMsg(msg, c, 0);
	printf("C: ");
	pbin(c, 64);
	printf("\n\n");
	encMsg(c, msg, 1);
	printf("M: ");
	pbin(msg, 64);
	printf("\n");

}

int main(int argc, char* argv[]) {
	int i;
	u8 key[8] = "12345678";
	u8 msg[8] = "01234567";

	if (argc == 2 && strcmp("-h", argv[1]) == 0) {
		printf("Usage: %s KEY MSG\n", argv[0]);
		return 0;
	}
	if (argc == 3) {
		if (strlen(argv[1]) == 8 && strlen(argv[2]) == 8) {
			strncpy((char *)key, argv[1], 8);
			strncpy((char *)msg, argv[2], 8);
		}
		else {
			fprintf(stderr, "invalid parameters: %s %s\n"
					"key and msg should be 8 bytes long\n",
					argv[1], argv[2]);
			return 1;
		}
	}
	des(key, msg);

	return 0;
}
