#import <Foundation/Foundation.h>
#import <stdlib.h>

int main(int argc, char *argv[]) {
	NSAutoreleasePool *const pool = [[NSAutoreleasePool alloc] init];
	NSUserDefaults *const defs = [NSUserDefaults standardUserDefaults];

	NSLog(@"cat toy: %@", [defs stringForKey:@"cattoy"]);
	NSLog(@"file name: %@", [defs stringForKey:@"filename"]);

	[pool drain];
	return EXIT_SUCCESS;
} // main
