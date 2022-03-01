#include <stdio.h>
#include <math.h>
#define N 100

// generates the n-element sequence by incrementing the start value
// using the step size
void range(double v[], int n, double start, double step)
{
	for(int i = 0; i < n; i++)
	{
		v[i] = start;
		start += step;
	}
}
// Returns n evenly spaced samples, calculated over the interval [start, stop].
// n >= 0
// for n = 0 return empty array
// for n = 1 return one-element array, with array[0] = start
void linspace(double v[], double start, double stop, int n)
{
	if(n==1)
	{
		v[0]=start;
	}
// nie rozumiemiem


}

// multiply each element of v by the value of scalar
void multiply_by_scalar(double v[], int n, double scalar)
{
	for(int i = 0; i < n; i++)
		v[i]*=scalar;
}

// add to each element v1[i] value of v2[i]
void add(double v1[], const double v2[], int n)
{
	for(int i =0;i<n;i++)
		v1[i] += v2[i];
	
}

// calculate and return the dot product of v1 and v2
double dot_product(const double v1[], const double v2[], int n)
{
	double sum = 0;
	for(int i =0;i<n;i++)
		sum += v1[i] * v2[i];
	return sum;
}

// read double vector of size n
void read_vector(double v[], int n)
{
	for(int i=0;i<n;i++)
	{
		double number;
		scanf("%lf",&number);
		v[i] = number;
	}
}

// print double vector of size n (with 2 significant figures)
void print_vector(const double v[], int n) {
	for (int i = 0; i < n; ++i) {
		printf("%.2f ", v[i]);
	}
	printf("\n");
}

int main(void) {

	int to_do, n;
	double start, stop, step, scalar;
	double vector_1[N], vector_2[N];

	scanf("%d", &to_do);

	switch (to_do) {
		case 1: // linspace
			scanf("%d %lf %lf", &n, &start, &stop);
			linspace(vector_1, start, stop, n);
			print_vector(vector_1, n);
			break;
		case 2: // add
			scanf("%d", &n);
			read_vector(vector_1, n);
			read_vector(vector_2, n);
			add(vector_1, vector_2, n);
			print_vector(vector_1, n);
			break;
		case 3: // dot product
			scanf("%d", &n);
			read_vector(vector_1, n);
			read_vector(vector_2, n);
			printf("%.2f\n", dot_product(vector_1, vector_2, n));
			break;
		case 4: // multiply by scalar
			scanf("%d %lf", &n, &scalar);
			read_vector(vector_1, n);
			multiply_by_scalar(vector_1, n, scalar);
			print_vector(vector_1, n);
			break;
		case 5: // range
			scanf("%d %lf %lf", &n, &start, &step);
			range(vector_1, n, start, step);
			print_vector(vector_1, n);
			break;
		default:
			printf("Unknown operation %d", to_do);
			break;
	}
	return 0;
}
