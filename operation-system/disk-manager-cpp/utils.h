#pragma once
#ifdef DEBUG
	#include <iostream>
	void DebugLog(const char *fmt, ...);
	#define DebugLog(fmt, ...) do { \
		printf("%s [%s:%s]" fmt "\n", __TIME__,  __FILE__, __FUNCTION__, ##__VA_ARGS__);\
	} while (0);
#else
	#define DebugLog(fmt, ...)
#endif
