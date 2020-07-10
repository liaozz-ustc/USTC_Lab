#define N 500000
#include<stdio.h>
#include<mpi.h>
#include<math.h>

int isPrime(int num) {
	int i,flag=1;
	int max = int(sqrt(num));
	for (i = 2; i <= max; i++) {
		if (num%i == 0) {
			flag = 0;
			break;
		}
	}
	return flag;
}

int main(int argc, char *argv[])

{
	int i, rank, size;
	double startTime, endTime;
	int localSum = 0, sum;

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	startTime = MPI_Wtime();
	for (i = rank; i <= N; i = i + size) {
		localSum += isPrime(i);
	}
	MPI_Reduce(&localSum, &sum, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
	if (rank == 0) {
		//移除0和1，0、1既不是素数也不是合数
		printf("素数个数为:%d\n", sum-2);
		endTime = MPI_Wtime();
		printf("运行时间:%fs\n", endTime - startTime);
	}
	MPI_Finalize();
	return 0;

}