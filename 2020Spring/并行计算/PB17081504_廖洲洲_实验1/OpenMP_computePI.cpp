#include<omp.h>
#include<stdio.h>
static long num_steps = 100000;
#define NUM_THREADS 8
void main() {
	int i;
	double x, sum, pi = 0;
	double step = 1.0 / num_steps;
	double startTime, endTime;
	omp_set_num_threads(NUM_THREADS);
	startTime = omp_get_wtime();
#pragma omp parallel private(i,x,sum)
	{
		int id = omp_get_thread_num();
		for (i = id, sum = 0; i < num_steps; i = i + NUM_THREADS) {
			x = (i + 0.5)*step;
			sum += 4.0 / (1.0 + x*x);
		}
#pragma omp critical
		pi += sum*step;
	}
	endTime = omp_get_wtime();
	printf("pi is %f\n", pi);
	printf("运行时间为:%f\n", endTime - startTime);
}