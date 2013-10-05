#include <stdio.h>

int main(void) {
	int n;

	do
	{
		scanf("%d", &n);
		if (n == 0)
			break;
		printf("%d\n", (n+1)/2);
	} while (1);

	return 0;
}
