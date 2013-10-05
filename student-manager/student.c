#include <stdio.h>
#include <stdlib.h>

#include "object.h"
#include "new.h"

#include "student.h"
#include "student.r.h"

static void* Student_ctor(void* __self, va_list *app)
{
	struct Student *self = ((const struct Class *)Person)->ctor(__self, app);

	self->ID = va_arg(*app, const int);
	return self;
}

static void* Student_dtor(void* __self)
{
	struct Student* self = __self;

	return self;
}

static int Student_differ(const void* a, const void* b)
{
	const struct Student* first = a;
	const struct Student* second = b;

	return first->ID - second->ID;
}

static int Student_puto(void* __self, FILE *fp)
{
	const struct Student* self = __self;
	
	fprintf(fp, "[%lu]\t", (unsigned long)self->ID);
	((const struct Class *)Person)->puto(__self, fp);
	return 0;
}

static const struct Class __Student =
{
	sizeof(struct Student),
	Student_ctor,
	Student_dtor,
	Student_differ,
	Student_puto
};

const void* Student = &__Student;
