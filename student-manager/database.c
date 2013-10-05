#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "new.h"
#include "object.h"

#include "database.h"
#include "database.r.h"
#include "database.d.h"

static void* Database_ctor(void* __self, va_list* app)
{
	struct Database* self = __self;
	const char* name = va_arg(*app, const char*);

	self->name = malloc(strlen(name) + 1);
	if (self->name == NULL)
	{
		perror("create database Failed!\n");
		exit(EXIT_FAILURE);
	}
	strcpy(self->name, name);
	self->count = 0;
	self->root = NULL;
#ifndef NDEBUG
	printf("===================================\n"
		"Database\n\tname: [%s]\n"
		"\tcount:  [%lu]\n"
		"===================================\n",
		self->name, (unsigned long)self->count
		);
#endif
	return self;
}


static void* Database_dtor(void* __self)
{
	struct Database *self = __self;
	struct Node* tmp = self->root;
	struct Node* p;

#ifndef NDEBUG
	printf("Database_dtor\n");
#endif
	free(self->name);
	self->name = NULL;
	self->count = 0;
#ifndef NDEBUG
	printf("clear\n");
#endif
	while (tmp != NULL)
	{
		p = tmp;
		tmp = tmp->next;
		delete(p->data);
		free(p);
	}
#ifndef NDEBUG
	printf("Cleared\n");
#endif

	return self;
}

static int Database_puto(void* __self, FILE *fp)
{
	struct Database *self = __self;

	fprintf(fp, "[%s]\t[%lu]\n", self->name, self->count);
	return 0;
}

static int Database_differ(const void* a, const void* b)
{
	const struct Database *first = a;
	const struct Database *second = b;

	return strcmp(first->name, first->name);
}

static const struct Class __Database =
{
	sizeof(struct Database),
	Database_ctor,
	Database_dtor,
	Database_differ,
	Database_puto
};

const void* Database = &__Database;


/*===============================[interface]=================================*/

void* add(void* __self, const void* __element)
{
	struct Database *self = __self;
	struct Node* root = self->root;
	struct Node* tmp = malloc(sizeof(struct Node));

	tmp->data = (void *)__element;
	tmp->prev = NULL;
	tmp->next = (struct Node *)self->root;
	self->root = tmp;
	++self->count;
	printf(">>>A new Record added to Database: [%lu]\n", self->count);
	return __self;
}


//not interface!!!
struct Node* ifind(const void* __self, const void* __element)
{
	const struct Database *self = __self;
	struct Node *tmp = self->root;
	unsigned long i = 0;

	while (i++ < self->count)
	{
		if (!differ((void *)tmp->data, (void *)__element))
			return tmp;
	}
	
	return NULL;
}

void* find(const void* __self, const void* __element)
{
	struct Node *result = ifind(__self, __element);
	if (result)
		return result->data;
	else
		return NULL;
}

void* drop(void* __self, const void* __element)
{
	struct Database *self = __self;
	struct Node *result = ifind(__self, __element);

	if (result)
	{
		result->prev = result->next;
		if (result->next)
			result->next->prev = result->prev;
		--self->count;
		delete(result->data);
		free(result);
		printf(">>>A record deleted from database: [%lu]\n", self->count);
	}
	else
		printf(">>>Not found the specified item\n");

	return __self;
}

/******************************************************************************/
struct Item
{
	struct Class __;
};

void save(void* __self)
{
	struct Database* self = __self;
	struct Node* tmp = self->root;
	struct Item *item;
	char buf[X_BUF_SIZE];
	FILE *fp;

	printf("Enter file name:\n");
	scanf("%s", buf);
	if ((fp = fopen(buf, "rb")) == NULL)
	{
		perror("Open file failed!");
	}
	while (tmp)
	{
		break;
	}
}

void load(void* __self)
{
	FILE *fp;
	char buf[X_BUF_SIZE];

	printf("Input database file name:\n");
	scanf("%s", buf);
	if ((fp = fopen(buf, "rb")) == NULL)
	{
		perror("Open file failed!");
	}
	fclose(fp);
}
