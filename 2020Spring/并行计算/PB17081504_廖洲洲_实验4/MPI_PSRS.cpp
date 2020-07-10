#include<stdio.h>
#include<mpi.h>
#include<time.h>
#include<stdlib.h>

#define N 10000000

void quickSort(double *num, int len) {
	//快速排序
	if (len <= 1) {
		return;
	}
	int i = 0, j = 0;
	double temp;
	double p = num[len - 1];
	for (j = 0; j < len - 1; j++) {
		if (num[j] < p) {
			temp = num[i];
			num[i] = num[j];
			num[j] = temp;
			i = i + 1;
		}
	}
	temp = num[len - 1];
	num[len - 1] = num[i];
	num[i] = temp;
	quickSort(num, i);
	quickSort(num + i + 1, len - i - 1);
}

void merge(double *num,int p,int q,int r) {
	//输入数组num，num[p]~num[q]有序，num[q+1]~num[r]有序，将num归并为一个有序数组
	int n1 = q - p + 1;
	int n2 = r - q;
	int len = n1 + n2;
	int i, j, k;
	double *temp = (double *)malloc(sizeof(double)*len);
	for (i = 0; i < len; i++) {
		temp[i] = num[i];
	}
	i = p;
	j = q + 1;
	for (k = 0; k < len&&i <=q && j <=r; k++) {
		if (temp[i] < temp[j]) {
			num[k] = temp[i];
			i++;
		}
		else {
			num[k] = temp[j];
			j++;
		}
	}
	if (i == q+1) {
		for (; k < len; k++) {
			num[k] = temp[j];
			j++;
		}
		return;
	}
	for (; k < len; k++) {
		num[k] = temp[i];
		i++;
	}
	return;
}

int main(int argc, char ** argv) {
	int rank, size;
	int i,j;
	int partition;
	double startTime, endTime;
	double *num = (double *)malloc(sizeof(double)*N);
	srand(time(NULL));
	//生成随机数字
	for (i = 0; i < N; i++) {
		num[i] = rand() * 10000 / RAND_MAX;
	}
	/*printf("Before sort:\n");
	for (i = 0; i < N; i++) {
		printf("%.2f  ", num[i]);
	}
	printf("\n");*/
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	/*if (rank == 0) {
		printf("Before sort:\n");
		for (i = 0; i < N; i++) {
			printf("%.2f  ", num[i]);
		}
		printf("\n");
	}*/
	startTime = MPI_Wtime();
	//均匀划分，将待处理数据均分为size份，进程rank处理下标rank*partition~(rank+1)*partiton-1段数组
	partition = N / size;
	int start = rank*partition;
	//局部排序，使用快速排序对本地数据进行排序
	quickSort(num + rank*partition, partition);
	//选取样本，从排序好的子序列中选取size个样本,并发送给0号进程，0号进程使用Gather收集数据
	double *cbuffer = (double *)malloc(sizeof(double)*size);
	int space = partition / size;
	for (i = 0; i < size; i++) {
		cbuffer[i] = (num + rank*partition)[i*space];
	}
	double *pivotbuffer = (double *)malloc(sizeof(double)*size*size);
	MPI_Barrier(MPI_COMM_WORLD);
	MPI_Gather(cbuffer, size, MPI_DOUBLE, pivotbuffer, size, MPI_DOUBLE, 0, MPI_COMM_WORLD);
	//样本排序，对选取的size*size个样本进行多次归并排序
	if (rank == 0) {
		/*for (i = 0; i < size*size; i++) {
			printf("%.2f  ", pivotbuffer[i]);
		}*/
		for (i = 1; i < size; i++) {
			merge(pivotbuffer, 0, i*size-1, (i + 1)*size-1);
		}
		/*printf("\n");
		for (i = 0; i < size*size; i++) {
			printf("%.2f  ", pivotbuffer[i]);
		}*/
		//选择size-1个主元
		for (i = 1; i < size; i++) {
			cbuffer[i] = pivotbuffer[i*size - 1];
		}
	}
	//使用广播将主元播送给其他进程
	MPI_Bcast(cbuffer, size, MPI_DOUBLE, 0, MPI_COMM_WORLD);
	/*printf("\n");
	for (i = 0; i < size; i++) {
		printf("%.2f  ", cbuffer[i]);
	}*/
	//主元划分，根据主元将本地数据划分为size段
	int *classStart = (int *)malloc(sizeof(int)*size);
	int *classLength = (int *)malloc(sizeof(int)*size);
	for (i = 0; i < size; i++) {
		classStart[i] = 0;
		classLength[i] = 0;
	}
	int index = 0;
	j = 1;
	for (i = 0; i < partition; i ++ ) {
		if (j<size&& num[start + i] > cbuffer[j]) {
			classStart[j] = i;
			j++;
			index++;
		}
		classLength[index]++;
	}
	/*printf("\n");
	for (i = 0; i < size; i++) {
		printf("@%d@start:%d,length:%d\n", rank,classStart[i], classLength[i]);
	}
	printf("@%d@\n", rank);
	for (i = 0; i < partition; i++) {
		printf("%.2f@%d@  ", num[start + i],rank);
	}*/
	//接收各段长度
	int *recvLength = (int *)malloc(sizeof(int)*size);
	int *recvStart = (int *)malloc(sizeof(int)*size);
	for (i = 0; i < size; i++) {
		MPI_Gather(classLength + i, 1, MPI_INT, recvLength, 1, MPI_INT, i, MPI_COMM_WORLD);
	}
	recvStart[0] = 0;
	//计算接收的各段的起始地址
	for (i = 1; i < size; i++) {
		recvStart[i] = recvStart[i - 1] + recvLength[i-1];
	}
	/*printf("\n");
	for (i = 0; i < size; i++) {
		printf("@%d@recvstart:%d,recvlength:%d\n", rank, recvStart[i], recvLength[i]);
	}*/
	//全局交换，各处理器将其有序段按段号交换到对应的处理器中
	double *recvBuffer = (double *)malloc(sizeof(double)*N);
	for (i = 0; i < size; i++) {
		MPI_Gatherv(num + start + classStart[i], classLength[i], MPI_DOUBLE, recvBuffer , recvLength, recvStart, MPI_DOUBLE, i, MPI_COMM_WORLD);
	}
	MPI_Barrier(MPI_COMM_WORLD);
	//归并排序，各处理器将收到的元素进行归并排序
	/*for (i = 0; i < N; i++) {
		printf("%.2f  ", recvBuffer[i]);
	}*/
	for (i = 1; i < size; i++) {
		merge(recvBuffer, 0, recvStart[i] - 1, recvStart[i]+recvLength[i]-1);
	}
	
	//收集，0号进程收集各进程处理的数据
	int len = 0;
	for (i = 0; i < size; i++) {
		len += recvLength[i];
	}
	/*for (i = 0; i < len; i++) {
		printf("__%.2f__  ", recvBuffer[i]);
	}*/
	//将各个进程归并处理的数据长度发给0号进程
	MPI_Gather(&len, 1, MPI_INT, recvLength, 1, MPI_INT, 0, MPI_COMM_WORLD);
	recvStart[0] = 0;
	for (i = 1; i < size; i++) {
		recvStart[i] = recvStart[i - 1] + recvLength[i-1];
	}
	MPI_Gatherv(recvBuffer, len, MPI_DOUBLE, num, recvLength, recvStart, MPI_DOUBLE, 0, MPI_COMM_WORLD);
	endTime = MPI_Wtime();
	if (rank == 0) {
		/*printf("After sort:\n");
		for (i = 0; i < N; i++) {
			printf("%.2f  ", num[i]);
		}*/
		printf("\n\nTime:%f\n", endTime - startTime);
	}
	MPI_Finalize();
	return 0;
}