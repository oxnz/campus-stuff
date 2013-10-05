// foldersize.m -- calculate the size of a folder with stat and getdirentiesattr

#import <dirent.h>	// for getdirentries()
#import <errno.h>
#import <fcntl.h>	// for O_RDONLY
#import <stdio.h>
#import <stdlib.h>
#import <string.h>
#import <sys/attr.h>	// for attrreference_t
#import <sys/dirent.h>	// for struct dirent
#import <sys/param.h>	// for MAXPATHLEN
#import <sys/stat.h>	// for struct statbuf and stat()
#import <sys/types.h>
#import <sys/vnode.h>	// for VDIR
#import <unistd.h>	// for getdirentriesattr()

// show the files and sizes of the files as they are processed
static int g_verbose = 0;

// stat code

// Calculate the directory size via stat()
off_t sizeForFolderStat (char *path) {
	DIR *directory = opendir(path);

	if (directory == NULL) {
		fprintf(stderr, "could not open directory '%s'\n", path);
		fprintf(stderr, "error is %d/%s\n", errno, strerror(errno));
		exit(EXIT_FAILURE);
	}

	off_t size = 0;
	struct dirent *entry;
	while ((entry = readdir(directory)) != NULL) {
		char filename[MAXPATHLEN];

		// don't mess with the metadirectories
		if (strcmp(entry->d_name, ".") == 0
				|| strcmp(entry->d_name, "..") == 0) {
			continue;
		}

		// Rather than changing the current working directory each
		// time through the loop, contruct the full path relative the
		// given path. Because the original path is either absolute,
		// or relative to the current working directory, this should
		// always give us a stat-able path
		snprintf(filename, MAXPATHLEN, "%s/%s", path, entry->d_name);

		// Use lstat so we don't multiply-count the sizes of files that
		// are pointed to by symlinks.
		struct stat statbuf;
		int result = lstat(filename, &statbuf);

		if (result != 0) {
			fprintf(stderr, "could not stat '%s': %s/%s\n",
					entry->d_name, errno, strerror(errno));
			continue;
		}

		// Recurse into subfolders
		if (S_ISDIR(statbuf.st_mode)) {
			size += sizeForFolderStat(filename);
		} else {
			if (g_verbose)
				printf("%lld %s\n", statbuf.st_size, entry->d_name);
			size += statbuf.st_size;
		}
	}

	closedir(directory);
	return size;
} // sizeForFolderStat

// getdirentiesattr code

// The attributes we want to get with each call to getdirentriesattr
static struct attrlist g_attrlist;	// get zeroed automatically

// The data being returned by each call
typedef struct fileinfo {
	u_int32_t	length;
	attrreference_t	name;
	fsobj_type_t	objType;
	off_t		logicalSize;
} fileinfo;

// try to pick up this many entries each time through
#define ENTRIES_COUNT 30

// Don't know how long each file name is, so make a guess so we can
// size the result buffer
#define AVG_NAME_GUESSTIMATE 64

off_t sizeForFolderAttr (char *path) {
	off_t size = 0;
