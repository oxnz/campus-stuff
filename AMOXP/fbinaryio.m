// fbinaryio.m -- do some binary reading and writing using buffered i/o

#import <errno.h>
#import <stdint.h>	// for uint32_t
#import <stdio.h>
#import <stdlib.h>
#import <string.h>

typedef struct Thing {
	uint32_t	thing1;
	float		thing2;
	char		thing3[8];
} Thing;

Thing things[] = {
	{ 3, 3.14159, "hello" },
	{ 4, 4.29301, "bye" },
	{ 2, 2.14214, "bork" },
	{ 5, 5.55556, "elf up" }
};

#define ErrH(err)	{ \
	fprintf(stderr, "%s: %d (%s)\n", \
			err, errno, strerror(errno)); \
	return EXIT_FAILURE; \
}

int main(int argc, char *argv[]) {
	size_t thingCount = sizeof(things) / sizeof(*things);
	FILE *file = fopen("/tmp/thingfile", "w");

	if (file == NULL)
		ErrH("error opening file");

	size_t numWrote = fwrite(things, sizeof(Thing), thingCount, file);

	if (numWrote != thingCount) {
		fprintf(stderr, "incomplete write (%d out of %d). Error %d (%s)\n", (int)numWrote, (int)thingCount, errno, strerror(errno));
		return EXIT_FAILURE;
	}

	fclose(file);

	// now re-open and re-read and make sure everything is groovy
	file = fopen("/tmp/thingfile", "r");

	if (file == NULL)
		ErrH("error opening file");

	// we know wr're reading in thingCount, so we can go ahead and
	// acquire that much space
	Thing readThings[sizeof(things)/sizeof(*things)];

	size_t numRead = fread(readThings, sizeof(Thing), thingCount, file);

	if (numRead != thingCount) {
		fprintf(stderr, "short read. Got %d, expected %d\n",
				(int)numRead, (int)thingCount);
		if (feof(file)) {
			fprintf(stderr, "we got an end of file\n");
		}
		if (ferror(file)) {
			fprintf(stderr, "we got an error: %d (%s)\n",
					errno, strerror(errno));
		}
	} else {
		// Just for fun, compare the newly read ones with the ones
		// we have statically declared
		for (int i = 0; i < thingCount; ++i) {
			if ((things[i].thing1 != readThings[i].thing1) ||
					(things[i].thing2 != readThings[i].thing2) ||
					(strcmp(things[i].thing3, readThings[i].thing3) != 0)) {
				fprintf(stderr, "mismatch with element %d\n", i);
			} else {
				printf("successfully compared element %d\n", i);
			}
		}
	}

	fclose(file);

	return EXIT_SUCCESS;
} // main
