/*******************************************************************************
 * dos2unix:
 * 		sed -i 's/\r//' file
 * 		cat file | col -b > newfile
 * 		cat file | tr -d '\r' > newfile
 * 		cat file | tr -d '\015' > newfile
 ******************************************************************************/
#include <stdio.h>
#include <stdlib.h>

int dos2unix(const char* file) {
	char ch;
	FILE* fp;

	if ((fp = fopen(file, "r")) == NULL) {
		perror("Cannot open input file");
		exit(EXIT_FAILURE);
	}
	while ((ch = fgetc(fp)) != EOF) {
		if (ch != '\r')
			putchar(ch);
	}
	return 0;
}

void help(void) {
	printf("Usage: dos2unix file"
			"\n"
	      );
}

int main(int argc, char* argv[]) {
	if (argc != 2) {
		help();
		exit(EXIT_FAILURE);
	}
	dos2unix(argv[1]);
	return EXIT_SUCCESS;
}
