// tempfun.m -- see how different temp file names are generated

#import <Foundation/Foundation.h>
#import <stdio.h>	// for the temp name functions
#import <stdlib.h>
#import <string.h>	// for strcpy()
#import <unistd.h>	// for mk[s]temp

int
main (int argc, char *argv[]) {
	NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];

	NSString *tempDir = NSTemporaryDirectory();
	if (tempDir == nil)
		tempDir = @"/tmp";
	NSLog(@"tempDir = %@", tempDir);

	NSString *template = [tempDir stringByAppendingPathComponent: @"bork.XXXXXX"];
	NSLog(@"template = %@", template);

	char *mutableTemplate = strdup([template fileSystemRepresentation]);
	char *path = mkdtemp(mutableTemplate);
	NSLog(@"after mkdtemp: %s", path);
	free(mutableTemplate);

	NSString *template2 =
		[tempDir stringByAppendingPathComponent: @"greebleXXXXXX.txt"];
	NSLog(@"template2 = %@", template2);

	mutableTemplate = strdup([template2 fileSystemRepresentation]);
	int fd = mkstemps(mutableTemplate, 4);
	NSLog(@"after mkstemps: fd %d, path %s", fd, mutableTemplate);
	free(mutableTemplate);

	close(fd);

	[pool drain];
	sleep(5);
	return EXIT_SUCCESS;
} // main
