#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "new.h"
#include "object.h"

#include "person.h"
#include "person.r.h"

static void* Person_ctor(void* __self, va_list* app)
{
	struct Person *self = __self;
	const char* name = va_arg(*app, const char*);
	const int age = va_arg(*app, const int);
	const char gender = (char)va_arg(*app, int);

	self->name = malloc(strlen(name) + 1);
	if (self->name == NULL)
	{
		perror("create person failed!\n");
		exit(EXIT_FAILURE);
	}
	strcpy(self->name, name);
	self->gender = gender;
	self->age = age;
#ifndef NDEBUG
	printf("Person name: [%s]\tage: [%d]\tgender:[%c]\n", self->name,
			(unsigned int)self->age, self->gender);
#endif
	return self;
}

static void* Person_dtor(void* __self)
{
	struct Person *self = __self;

#ifndef NDEBUG
	printf("Person delete...\n");
#endif
	free(self->name);
	self->name = NULL;
	self->age = 0;
	return self;
}

static int Person_differ(const void* a, const void* b)
{
	const struct Person *first = a;
	const struct Person *second = b;

	return strcmp(first->name, second->name);
}

static int Person_puto(void* __self, FILE *fp)
{
	const struct Person* self = __self;

	fprintf(fp, "[%s]\t[%lu]\t[%c]\n", self->name, (unsigned long)self->age,
			self->gender);
	return 0;
}

static const struct Class __Person =
{
	sizeof(struct Person),
	Person_ctor,
	Person_dtor,
	Person_differ,
	Person_puto
};

const void* Person = &__Person;
