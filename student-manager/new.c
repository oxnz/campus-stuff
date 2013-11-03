#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "new.h"
#include "object.h"

Object new(Type __class, ...)
{
	const struct Class* class = __class;
	void* p = calloc(1, class->size);

	if (p == NULL)
	{
		perror("create object failed!\n");
		exit(EXIT_FAILURE);
	}
	*(const struct Class **)p = class;

	if (class->ctor)
	{
		va_list ap;

		va_start(ap, __class);
		p = class->ctor(p, &ap);
		va_end(ap);
	}
	return p;
}

void delete(Object __self)
{
	const struct Class **cp = __self;

#ifndef NDEBUG
	printf("delete...\n");
#endif
	if (__self && *cp && (*cp)->dtor)
		__self = (*cp)->dtor(__self);
	free(__self);
#ifndef NDEBUG
	printf("delete...END\n");
#endif
}

size_t sizeOf(Object __self)
{
	const struct Class * const *cp = __self;

	if (!(__self && *cp))
	{
		fprintf(stderr, "%s", "NULL pointer detected!\n");
		exit(EXIT_FAILURE);
	}
	return (*cp)->size;
}

int differ(const Object __self, const Object b)
{
	const struct Class * const * cp = __self;

	if (__self && *cp && (* cp)->differ)
		return (*cp)->differ(__self, b);
    return 1;
}

int puto(Object __self, FILE *fp)
{
	const struct Class * const *cp = __self;

	if (cp && *cp && (*cp)->puto)
		(*cp)->puto(__self, fp);
	else
		printf("Can NOT be output\n");
	return 0;
}
