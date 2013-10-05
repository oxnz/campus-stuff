// readvecstring.m -- open /tmp/stringfile.txt and write out its content
//			using scatter/gather reads

#import <errno.h>
#import <fcntl.h>
#import <stdint.h>
#import <stdio.h>
#import <stdlib.h>
#import <string.h>
#import <sys/stat.h>	// for permission flags
#import <sys/types.h>	// for ssize_t
#import <sys/uio.h>	// for readv() and struct iovec
#import <unistd.h>	// for close()

#define errorHandle(status, flag, errMsg) if (status == flag) { \
	fprintf(stderr, "%s. Error %d (%s)\n",	\
			errMsg, errno, strerror(errno));	\
	return EXIT_FAILURE;	\
}

int main(int argc, char *argv[]) {
	int fd = open("/tmp/stringfile.txt", O_RDONLY);

	errorHandle(fd, -1, "can't open file");

	uint32_t stringLength;
	char buffer[4096];
	struct iovec vector[2];

	vector[0].iov_base = &stringLength;
	vector[0].iov_len = sizeof(stringLength);
	vector[1].iov_base = buffer;
	vector[1].iov_len = sizeof(buffer) - 1;

	ssize_t result = readv(fd, vector, 2);

	errorHandle(result, -1, "can't read file");

	buffer[stringLength] = '\0';	// need to zero-terminate it

	close(fd);

	printf("out string is '%s'\n", buffer);

	return EXIT_SUCCESS;
}
