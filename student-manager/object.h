#ifndef X_CLASS_R
#define X_CLASS_R

#include <stdarg.h>

//#define NDEBUG

struct Class
{
	size_t size;
	void* (* ctor)(void*, va_list*);
	void* (* dtor)(void*);
	int (* differ)(const void*, const void*);
	int (*puto)(void*, FILE *fp);
};

#endif
