// longjmp.m -- use setjmp, longjmp
#import <setjmp.h>	// for setjmp/longjmp
#import <stdio.h>
#import <stdlib.h>

static jmp_buf handler;

void doEvenMoreStuff() {
	printf("\t\tentering doEvenMoreStuff\n");
	printf("\t\tdone with doEvenMoreStuff\n");
} // doEvenMoreStuff

void doMoreStuff() {
	printf("\tentering doMoreStuff\n");
	doEvenMoreStuff();
	longjmp(handler, 23);
	printf("\tdone with doMoreStuff\n");
} // doMoreStuff

void doStuff() {
	printf("entering doStuff\n");
	doMoreStuff();
	printf("done with doStuff\n");
} // doStuff

int main(void) {
	int result;

	if ((result = setjmp(handler))) {
		printf("longjump called, setjmp returned again: %d\n", result);
	} else {
		doStuff();
	}

	return EXIT_SUCCESS;
} // main
