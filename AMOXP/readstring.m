// readstring.m -- open /tmp/stringfile.txt and write out its contents

#import <errno.h>
#import <fcntl.h>
#import <stdio.h>
#import <stdlib.h>
#import <stdint.h>	// for uint32_t
#import <string.h>
#import <unistd.h>

int main(int argc, char *argv[]) {
	int fd = open("/tmp/stringfile.txt", O_RDONLY);

	if (fd == -1) {
		fprintf(stderr, "can't open file. Error %d (%s)\n",
				errno, strerror(errno));
		return EXIT_FAILURE;
	}

	uint32_t stringLength;
	ssize_t result = read(fd, &stringLength, sizeof(stringLength));

	if (result == -1) {
		fprintf(stderr, "can't read file. Error %d (%s)\n",
				errno, strerror(errno));
		return EXIT_FAILURE;
	}

	// +1 accounts for the trailing zero byte we'll be adding to terminate
	// the string
	char *buffer = malloc(stringLength + 1);

	result = read(fd, buffer, stringLength);

	if (result == -1) {
		fprintf(stderr, "can't read file. Error %d (%s)\n",
				errno, strerror(errno));
		return EXIT_FAILURE;
	}

	buffer[stringLength] = '\0';

	close(fd);

	printf("the string is '%s'\n", buffer);

	free(buffer);	// clean up our mess

	return EXIT_SUCCESS;
} // main
