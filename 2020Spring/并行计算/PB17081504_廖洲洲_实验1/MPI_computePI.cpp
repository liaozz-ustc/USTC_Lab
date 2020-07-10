#define N 100000
#include"mpi.h"
#include<stdio.h>
#include<math.h>

int main(int argc, char ** argv)
{
	double local = 0, pi, w, temp;
	int i, rank, size;
	double startTime, endTIme;
	w = 1.0 / N;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	startTime = MPI_Wtime();
	for (i = rank; i < N; i = i + size) {
		temp = (i + 0.5)*w;
		local = 4.0 / (1 + temp*temp) + local;
	}
	MPI_Reduce(&local, &pi, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
	if (rank == 0) {
		printf("pi is %f\n", pi*w);
		endTIme = MPI_Wtime();
		printf("运行时间:%fs\n", endTIme - startTime);
	}
	MPI_Finalize();
	return 0;
}