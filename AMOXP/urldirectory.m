// urldirectory.m -- Use URLForDirectory:inDomain:etc

#import <Foundation/Foundation.h>

int main (int argc, char *argv[]) {
	NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];

	NSURL *appropriateURL = nil;
	if (argc == 2) {
		NSString *path = [NSString stringWithUTF8String: argv[1]];
		appropriateURL = [NSURL fileURLWithPath: path];
	}

	NSFileManager *fm = [[[NSFileManager alloc] init] autorelease];

	NSError *error;
	NSURL *directoryURL =
		[fm URLForDirectory: NSItemReplacementDirectory
			   inDomain:NSUserDomainMask
		  appropriateForURL:appropriateURL
			     create:YES
			      error:&error];

	if (directoryURL == nil) {
		NSLog(@"Could not get directory URL. Error %@", error);
	} else {
		NSLog(@"directoryURL is %@", directoryURL);
	}

	[pool drain];
	return 0;
} // main
