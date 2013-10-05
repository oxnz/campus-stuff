#import <Foundation/Foundation.h>	// for BOOL
#import <stdio.h>	// for standard I/O stuff
#import <stdlib.h>	// for EXIT_FAILURE/SUCCESS
#import <fnmatch.h>	// for fnmatch()

#define BUFFER_SIZE	(2048)

void changeCaseBuffer (char buffer[], size_t length, BOOL upcase) {
	char *scan = buffer;
	char *const stop = buffer + length;

	while (scan < stop) {
		*scan = upcase ? toupper(*scan) : tolower(*scan);
		scan++;
	}
} // changeCaseBuffer


int main (int argc, char *argv[]) {
	char buffer[BUFFER_SIZE];
	BOOL upcase = YES;

	if (fnmatch("*upcase", argv[0], FNM_NOESCAPE) == 0) {
		fprintf(stderr, "upcase!\n");
		upcase = YES;
	} else if (fnmatch("*downcase", argv[0], FNM_NOESCAPE) == 0) {
		fprintf(stderr, "downcase!\n");
		upcase = NO;
	}

	while (!feof(stdin)) {
		const size_t length = fread(buffer, 1, BUFFER_SIZE, stdin);
		changeCaseBuffer(buffer, length, upcase);
		fwrite(buffer, 1, length, stdout);
	}

	return EXIT_SUCCESS;
} // main
