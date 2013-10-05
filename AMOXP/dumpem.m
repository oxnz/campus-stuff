// dumpem.m -- look into the keychain
#import <Security/Security.h>
#import <Foundation/Foundation.h>

#import <stdio.h>
#import <stdlib.h>
#import <string.h>

// Given a carbon-style four character code, make a C string that can
// be given to printf
const char* fourByteCodeString (UInt32 code) {
	// Splat the bytes of the four character code into a character array.
	typedef union theCheat {
		UInt32 code;
		char string[4];
	} theCheat;

	// C string that gets returned.
	static char string[5];

	// Byte-swap, otherwise string will be backwards on little endian system
	((theCheat *)string)->code = ntohl(code);
	string[4] = '\0';
	return (string);
} // fourByteCodeString

// Display each attribute in the list.
void showList(SecKeychainAttributeList list) {
	for (int i = 0; i < list.count; i++) {
		SecKeychainAttribute attr = list.attr[i];

		char buffer[1024];
		if (attr.length < sizeof(buffer)) {
			// make a copy of the data so we can stick on
			// a trailing zero byte
			strncpy(buffer, attr.data, attr.length);
			buffer[attr.length] = '\0';
			
			printf("\t%d: '%s' = \"%s\"\n", i, fourByteCodeString(attr.tag), buffer);
		} else {
			printf("attribute %s is more than 1K\n", fourByteCodeString(attr.tag));
		}
	}
}// showList

// Display a keychain item's info.
void dumpItem (SecKeychainItemRef item, bool displayPasswork) {
	// Build the attributes we're interest in examining.
	SecKeychainAttribute attributes[3];
	attributes[0].tag = kSecAccountItemAttr;
	attributes[1].tag = kSecDescriptionItemAttr;
	attributes[2].tag = kSecModDateItemAttr;

	SecKeychainAttributeList list;
	list.count = 3;
	list.attr = attributes;

	// Get the item's information, including the password.
	UInt32 length = 0;
	char *password = NULL;
	OSErr result;
	if (displayPasswork) {
		result = SecKeychainItemCopyContent (item, NULL, &list, &length,
				(void **)&password);
	} else {
		result = SecKeychainItemCopyContent (item, NULL, &list, NULL, NULL);
	}

	if (result != noErr) {
		printf("dumpItem: error result of %d\n", result);
		return; 
	}

	if (password != NULL) {
		// Copy the password into a buffer and attach a trailing zero
		// byte so we can print it out with printf.
		char *passwordBuffer = malloc(length + 1);
		strncpy(passwordBuffer, password, length);

		passwordBuffer[length] = '\0';
		printf("Password = %s\n", passwordBuffer);
		free(passwordBuffer);
	}

	showList(list);
	SecKeychainItemFreeContent (&list, password);
} // dumpItem

int main(int argc, char *argv[]) {
	NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
	if (argc != 2) {
		printf("usage: %s account-name\n", argv[0]);
		return EXIT_FAILURE;
	}

	// Biuld an attribute list with just one attribute
	SecKeychainAttribute attribute;
	attribute.tag = kSecAccountItemAttr;
	attribute.length = strlen(argv[1]);
	attribute.data = argv[1];

	// Make a list to point to this new attribute.
	SecKeychainAttributeList list;
	list.count = 1;
	list.attr = &attribute;

	// Create a search handle with the attribute list.
	SecKeychainSearchRef search;
	OSErr result = SecKeychainSearchCreateFromAttributes
		(NULL, kSecInternetPasswordItemClass, &list, &search);

	if (result != noErr) {
		printf("result %d from SecKeychainSearchCreateFromAttributes\n", result);
	}

	// Iterate over the search result
	int count = 0;
	SecKeychainItemRef item;
	while (SecKeychainSearchCopyNext(search, &item) != errSecItemNotFound) {
		dumpItem (item, true);
		CFRelease(item);
		count++;
	}

	printf("%d items found\n", count);
	CFRelease(search);
	[pool drain];

	return EXIT_SUCCESS;
} // main
