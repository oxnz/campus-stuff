#include <stdio.h>
#include <math.h>

int main(void) {

	int T;
	scanf("%d", &T);
	int n;

	while (T--) {
		scanf("%d", &n);
		if ((n / 13)%2)
			printf("flymouse wins the game!\n");
		else if (n > 13 < 28)
			printf("snoopy wins the game!\n");
		else if (n > 28 < 40)
			printf("flymouse wins the game!\n");
		else
			printf("snoopy wins the game!\n");
	}
	return 0;
}
