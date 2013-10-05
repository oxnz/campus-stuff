// buffread.m -- show how to read using buffered I/O, including error / eof
//		handling

#import <stdlib.h>
#import <stdio.h>	// all the buffered I/O API

int main (int argc, char *argv[]) {
	FILE *file = fopen("buffered.m", "r");

	while (1) {
		int result = fgetc(file);
		if (result == EOF) {
			if (feof(file)) {
				printf("EOF found\n");
			}
			if (ferror(file)) {
				printf("error reading file\n");
			}
			break;
		} else {
			printf("got a character: '%c'\n", (char) result);
		}
	}

	fclose(file);

	return EXIT_SUCCESS;
} // main
