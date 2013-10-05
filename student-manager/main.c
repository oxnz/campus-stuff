#include <stdio.h>
#include <stdlib.h>

#include "database.h"
#include "person.h"
#include "student.h"
#include "new.h"

#include "common.h"
#include "utility.h"

void optDB(void*, Choice* mask);
void optRecord(Choice* mask);

int main(int argc, char *argv[])
{
	Choice mask = 0;
	void* db;

	if (argc != 2)
	{
		usage(argv[0]);
		exit(EXIT_SUCCESS);
	}
	printf("Welcome to the STU-Manager v0.1!\n");
	db = new(Database, argv[1]);
	optDB(db, &mask);

	return 0;
}

void optDB(void* db, Choice* mask)
{
	void* a = new(Person, "John", 12, 'M');
	void* aa= new(Person, "John", 12, 'M');
	void* b = new(Person, "Mirson", 21, 'F');
	void* c= new(Student, "Happy",  5, 'M', 10101);
	void* d = new(Student, "Friend", 6, 'F', 20101); void* e = new(Student, "Friend", 6, 'F', 20101); Choice choice;

	while ((choice = getOpt(mainMenu, 9)) != 0)
	{
		choice &= ++(*mask);
#ifndef NDEBUG
			printf("mask>>>>>> [%d]\n", *mask);
			printf("choice>>>> [%d]\n", choice);
#endif
		switch(choice)
		{
			case 0:
				load(db);
				break;
			case 2:
				printf("Load\n");
				break;
			case 3:
				optRecord(mask);
				break;
			case 4:
				puto(db, stdout);
				break;
			case 5:
			case 6:
			case 7:
				break;
			case 8:
				help();
				break;
			case 9:
				about();
				break;
			default:
				printf("Unknow option, try again:\n");
				break;
		}
	}

	printf("===================construction completed==================\n");
	puto(db, stdout);
	if (!add(db, a))
		printf("Error add\n");
	add(db, c);
	add(db, d);
	puto(db, stdout);
	printf("-----------------------------------------------------------\n");
	if (a)
		drop(db, a);
	puto(db, stdout);
	printf("-----------------------------------------------------------\n");

	if (differ(aa, b))
		printf("a is Different from b\n");
	printf("-----------------------------------------------------------\n");
	if (differ(e, d))
		printf("Student are different\n");
	else
		printf("The same student\n");
	printf("-----------------------------------------------------------\n");
	puto(db, stdout);
	puto(aa, stdout);
	puto(c, stdout);
	puto(d, stdout);
	printf("sizeOf(db): [%lu]\n", (unsigned long)sizeOf(db));
	printf("sizeOf(aa): [%lu]\n", (unsigned long)sizeOf(aa));
	delete(b);
	printf("delete db\n");
	delete(db);
	printf("End of Test\n");
}

void optRecord(Choice* mask)
{
	Choice choice;
	while (choice = getOpt(subMenu, 5) != 0)
	{
		switch(getOpt(subMenu, 5))
		{
			case 1:
				printf("FUCK\n");
				break;
			default:
				printf("Unknow option, try again:\n");
				break;
		}
	}
}
