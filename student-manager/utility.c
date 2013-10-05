#include "utility.h"

#include <stdio.h>

void usage(char *prog)
{
	printf("%s [DBname]\n", prog);
}

void about(void)
{
	printf("===========[About Stu_manager System]===========\n"
	       "\tAuthor:	yunxinyi@gmail.com\n"
	       "\tBug report:	yunxinyi@gmail.com\n"
	       "\tRelease:	0.1-alpha\n"
	      );
}

void help(void)
{
	printf("===========[Stu_manager System]===========\n"
	       "Help:\n"
	       "\t1\n"
	       "\t2\n"
	      );
}
