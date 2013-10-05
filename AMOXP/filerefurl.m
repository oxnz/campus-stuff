// filerefurl.m -- Play with file reference urls.

#import <Foundation/Foundation.h>

int main (void) {
	NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];

	// Create the file.
	system ("touch /tmp/oopack");
	NSURL *url = [NSURL fileURLWithPath: @"/tmp/oopack"];
	NSURL *fileReference = [url fileReferenceURL];

	NSLog(@"url: %@ : %@", url, url.path);
	NSLog(@"ref: %@ : %@", fileReference, fileReference.path);

	NSLog(@"moving the file");
	system("mv /tmp/oopack /tmp/jo2yfund");

	NSLog(@"url: %@ : %@", url, url.path);
	NSLog(@"ref: %@ : %@", fileReference, fileReference.path);

	unlink("/tmp/jo2yfund");

	[pool drain];
	return EXIT_SUCCESS;
} // main
