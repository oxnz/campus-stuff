#include <stdio.h>

int main(void) {
	int w[4][4] = {
		{0, 1, 2, 2},
		{1, 0, 1, 1},
		{2, 1, 0, 2},
		{2, 1, 2, 0},
	};
	int l = 2;
	int r = 3;
	int x;
	int sum = 0;
	
	while ((x = getchar()) != EOF) {
		x -= 0x30;
		if (x - l | x - r) {
			if (w[l][x] < w[r][x])
			{
				sum += w[l][x];
				l = x;
			}
			else
			{
				sum += w[r][x];
				r = x;
			}
		}
	}
	printf("%d\n", sum);

	return 0;
}
