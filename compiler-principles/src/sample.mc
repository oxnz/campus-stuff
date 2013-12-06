/* File: sample.input
 * Author: Oxnz
 * Description: Test Microcc
 */

int add(int a, int b) {
	return a + b;
}

int sub (int a, int b ) {
	return a - b;
}

double mul(double a, double b);

int abs(int a) {
	if (a < 0)
		a = -a;
	return a;
}

int main(int argc, char* argv[]) {
	int i = 10;
	int j = 20;
	int k = i - j * 10;
	k = abs(k - i);
	j = k + i;

	int a[20] = {0, 1, 2, 3};
	double pi = 3.14;
	echo(k); // echo the result to screen
	echo (j);
	echo ("This is a test string");

	return 0; /* return 0 stands for success */
}

double mul(double a, double b) {
	return a*b;
}
