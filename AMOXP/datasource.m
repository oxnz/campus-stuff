// datasource.m -- look at ObjC2 protocol addtions.

#import <Foundation/Foundation.h>

// Datasource for the new table view
@protocol NewTableViewDataSource
- (NSUInteger) rowCount;	// defaults to being required

@optional
- (BOOL) shouldEncheferizeStrings;
- (NSIndexSet *) emptyRows;

@required
- (id) dataValueAtRow: (NSUInteger) row;
@end	// NewTableViewDataSource protocol

// a datasource class

@interface DataSource : NSObject <NewTableViewDataSource>
@end	// DataSource

@implementation DataSource

- (NSUInteger) rowCount {
	return 23;
} // rowCount

- (id) dataValueAtRow: (NSUInteger) row {
	return [NSNumber numberWithInt: row * 7];
} // dataValueAtRow

- (BOOL) shouldEncheferizeStrings {
	return YES;	// bork bork bork
} //should EncheferizeStrings

@end // DataSource

// the new table view
@interface NewTableView : NSObject {
	id datasource;
}

- (void) setDataSource: (id <NewTableViewDataSource>) ds;
- (void) doStuff;

@end // NewTableView

@implementation NewTableView

- (void) setDataSource: (id <NewTableViewDataSource>) ds {
	datasource = ds;
} // setDataSource

- (void) doStuff {
	// Don't need to check for respondsToSelector - the compiler does
	// some sanity checking for us.
	NSLog (@"rowCount: %u", [datasource rowCount]);
	NSLog(@"value at row 5: %@", [datasource dataValueAtRow: 5]);

	// These are optional, so check that the datasource responds to them.
	if ([datasource respondsToSelector: @selector(shouldEncheferizeStrings)]) {
		NSLog(@"bork bork bork? %@",
				[datasource shouldEncheferizeStrings] ? @"YES" : @"NO");
	}
	if ([datasource respondsToSelector: @selector(emptyRows)]) {
		    NSLog(@"the empty rows: %@", [datasource emptyRows]);
	}
} // doStuff
@end // NewTableView

// Use everything.

int main(void) {
	NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];

	NewTableView *tableView = [[NewTableView alloc] init];

	DataSource *ds = [[DataSource alloc] init];

	[tableView setDataSource: [NSNull null]];	// should warn
	[tableView setDataSource: ds];
	[tableView doStuff];

	[pool drain];
	return 0;
} // main
