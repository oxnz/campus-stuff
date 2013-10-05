// writevecstring.m -- take argv[1] and write it to a file, prepending the
// 			length of the string. and using scatter/gather I/O

#import <errno.h>
#import <fcntl.h>
#import <stdint.h>
#import <stdio.h>
#import <stdlib.h>
#import <string.h>
#import <sys/stat.h>	// for permission flags
#import <sys/types.h>	// for ssize_t
#import <sys/uio.h>	// for writev() and struct iovec
#import <unistd.h>	// for close()

int main(int argc, char *argv[]) {
	if (argc != 2) {
		fprintf(stderr, "usage: %s string-to-log\n", argv[0]);
		return EXIT_FAILURE;
	}

	int fd = open("/tmp/stringfile.txt", O_WRONLY | O_CREAT | O_TRUNC,
			S_IRUSR | S_IWUSR);

	if (fd == -1) {
		fprintf(stderr, "can't open file. Error %d (%s)\n",
				errno, strerror(errno));
		return EXIT_FAILURE;
	}

	uint32_t stringLength = strlen(argv[1]);

	struct iovec vector[2];	// one for size, one for string
	vector[0].iov_base = &stringLength;
	vector[0].iov_len = sizeof(stringLength);
	vector[1].iov_base = argv[1];
	vector[1].iov_len = stringLength;

	size_t result = writev(fd, vector, 2);

	if (result == -1) {
		fprintf(stderr, "can't write to file. Error %d (%s)\n",
				errno, strerror(errno));
		return EXIT_FAILURE;
	}

	result = close(fd);
	if (result == -1) {
		fprintf(stderr, "can't close file. Error %d (%s)\n",
				errno, strerror(errno));
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
} // main
