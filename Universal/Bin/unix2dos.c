/*******************************************************************************
 * unix2dos:
 * 		sed -i 's/$/\r/' file
 * 		sed -i 's/$/\x0d/' file
 ******************************************************************************/
#include <stdio.h>
#include <stdlib.h>

int unix2dos(const char* file) {
	int ch;
	FILE* fp;

	if ((fp = fopen(file, "r")) == NULL) {
		perror("Cannot open input file");
		exit(EXIT_FAILURE);
	}

	while ((ch = fgetc(fp)) != EOF) {
		if (ch == '\n')
			putchar('\r');
		putchar(ch);
	}
	fclose(fp);
	return 0;
}

void help(void) {
	printf("Usage: unix2dos file\n"
		"convert unix text format to dos format\n"
		"\t-h\tshow this message and exit.\n"
			"\n"
	      );
}

int main(int argc, char* argv[]) {
	if (argc != 2) {
		help();
		exit(EXIT_FAILURE);
	}
	unix2dos(argv[1]);
	return EXIT_SUCCESS;
}
