// debuglog.m -- a function for conditional logging

#import <stdio.h>
#include <stdarg.h>

int globalLevel = 50;

void debugLog(int logLevel, const char *format, ...)
{
	if (logLevel > globalLevel) {
		va_list args;
		va_start(args, format);
		vprintf(format, args);
		va_end(args);
	}
} // debugLog

int main(int argc, char *argv[])
{
	debugLog(10, "this will not be seen: %d, %s, %d\n",
			10, "hello", 23);
	debugLog(87, "this would be seen: %s, %d\n",
			"bork", 42);

	return 0;
} // main
