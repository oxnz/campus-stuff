// stringization.m -- show token stringization and concatenation

/* The # character can be used in a macro definition to turn an argument into a
 * literal string rather than having it evaluated. ## can be used to textually
 * paste two tokens together.
 */

#import <stdio.h>	// for printf()
#import <math.h>	// for sqrt()

#define FIVE 5

int main(void) {
#define PRINT_EXPR(x) printf("%s = %d\n", #x, (x))
	PRINT_EXPR (5);
	PRINT_EXPR (5 * 10);
	PRINT_EXPR ((int)sqrt(FIVE*FIVE) + (int)log(25/5));

#define SPLIT_FUNC(x,y) x##y
	SPLIT_FUNC(prin, tf) ("hello\n");

	return 0;
} // main
