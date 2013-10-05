// cheapls.m -- a featureless ls program using the directory iteration functions

#import <dirent.h>	// for opendir and friends
#import <errno.h>
#import <stdio.h>
#import <stdlib.h>
#import <string.h>
#import <sys/dirent.h>	// for struct dirent
#import <sys/types.h>

int main (int argc, char *argv[]) {
	if (argc != 2) {
		fprintf(stderr, "usage: %s /path/to/directory\n", argv[0]);
		return EXIT_FAILURE;
	}

	DIR *directory = opendir(argv[1]);
	if (directory == NULL) {
		fprintf(stderr, "could not open directory '%s'\n", argv[1]);
		fprintf(stderr, "error is useful: %d (%s)\n",
				errno, strerror(errno));
		return EXIT_FAILURE;
	}

	struct dirent *entry;
	printf("position\tfileno\treclen\ttype\tnamelen\tname\n");
	while ((entry = readdir(directory)) != NULL) {
		long position = telldir(directory);
		printf("%3ld\t%llu\t%u\t0x%x\t%u\t%s\n",
				position, entry->d_fileno, entry->d_reclen,
				entry->d_type, entry->d_namlen, entry->d_name);
	}

	int result = closedir(directory);
	if (result == -1) {
		fprintf(stderr, "error closing directory: %d (%s)\n",
				errno, strerror(errno));
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
} // main
