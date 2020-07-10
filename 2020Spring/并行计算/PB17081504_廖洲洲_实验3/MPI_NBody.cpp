#include<stdio.h>
#include<mpi.h>
#include<stdlib.h>
#include<math.h>

#define N 256				//小球个数
#define G 6.67e-11			//万有引力常数
#define M 10000				//小球质量
#define Cycle 500000			//总的周期数，每周期时间=TotalTime/Cycle
#define CycleTime 0.01		//每周期的时间

struct ball {
	double ax, ay;			//加速度
	double vx, vy;			//速度
	double px, py;			//位置
};

void compute_force(int index, ball *ballList) {
	//计算编号为index的小球所受到的加速度
	//初始化加速度为0，再遍历其它小球，计算新的加速度
	double px, py;
	double dx, dy,d;
	px = ballList[index].px;
	py = ballList[index].py;
	ballList[index].ax = 0;
	ballList[index].ay = 0;
	for (int i = 0; i < N; i++) {
		if (i != index) {
			dx = ballList[i].px - px;
			dy = ballList[i].py - py;
			d = sqrt(dx*dx + dy*dy);
			if (d < 0.001) {
				//如果两球距离过小，使用1cm作为距离计算
				d = 0.001;
				dx = 0.001*dx / d;
				dy = 0.001*dy / d;
			}
			ballList[index].ax += G*M*dx / (d*d*d);
			ballList[index].ay += G*M*dy / (d*d*d);
		}
	}
}

void compute_velocities(int index, ball * ballList) {
	//计算每个小球的速度
	ballList[index].vx += ballList[index].ax*CycleTime;
	ballList[index].vy += ballList[index].ay*CycleTime;
}

void compute_positions(int index, ball * ballList) {
	//计算每个小球的位置
	double oldVx, oldVy;
	//保存旧的速度值
	oldVx = ballList[index].vx;
	oldVy = ballList[index].vy;
	compute_velocities(index, ballList);
	ballList[index].px += (oldVx + ballList[index].vx)*CycleTime / 2.0;
	ballList[index].py += (oldVy + ballList[index].vy)*CycleTime / 2.0;
}


int main(int argc, char ** argv) {
	ball ballList[N];
	int i, j;
	int rank, size,partition,low,high;
	double startTime, endTime;
	
	//初始化小球
	int temp = int(sqrt(N));
	int index;
	for (i = 0; i < temp; i++) {
		for (j = 0; j < temp; j++) {
			index = i*temp + j;
			ballList[index].px = j*0.01;
			ballList[index].py = i*0.01;
			ballList[index].ax = 0;
			ballList[index].ay = 0;
			ballList[index].vx = 0;
			ballList[index].vy = 0;
		}
	}
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	startTime = MPI_Wtime();
	//每个周期计算小球的位置，周期结束时向其他进程广播本进程负责的小球位置
	partition = N / size;//负责的小球个数
	low = rank*partition;//负责的小球的最小编号
	high = (rank + 1)*partition - 1;//负责的小球的最大编号
	//printf("$$$$$$$$$$$$$$\n");
	//for (j = 0; j < N; j++) {
	//	printf("rank %d,ball %d,(%f,%f)\n", rank, j, ballList[j].px, ballList[j].py);
	//}
	//printf("$$$$$$$$$$$$$$\n");
	for (i = 0; i < Cycle; i++) {
		for (j = low; j <= high; j++) {
			//首先更新各个小球在当前位置受到的力，计算出加速度
			compute_force(j, ballList);
		}
		for (j = low; j <= high; j++) {
			//计算出各个小球新的位置
			compute_positions(j, ballList);
		}
		//每个进程广播自己负责的小球信息，实验了数据同步
		//printf("#############\n");
		//for (j = 0; j < N; j++) {
		//	printf("rank %d,ball %d,(%f,%f)\n", rank, j, ballList[j].px, ballList[j].py);
		//}
		//printf("#############\n");
		for (j = 0; j < size; j++) {
			//每个进程广播自己负责的小球信息，实验了数据同步
			MPI_Bcast(&ballList[j*partition], sizeof(ball)*partition, MPI_BYTE, j, MPI_COMM_WORLD);
		}
		MPI_Barrier(MPI_COMM_WORLD);
	}
	//printf("***************\n");
	//for (j = 0; j < N; j++) {
	//	printf("rank %d,ball %d,(%f,%f)\n", rank, j, ballList[j].px, ballList[j].py);
	//}
	//printf("***************\n");
	endTime = MPI_Wtime();
	if (rank == 0) {
		printf("运行时间:%f\n", endTime - startTime);
		/*printf("球最终位置\n");
		for (i = 0; i < temp; i++) {
			for (j = 0; j < temp; j++) {
				index = i*temp + j;
				printf("\t(%.2f,%.2f)", ballList[index].px, ballList[index].py);
			}
			printf("\n");
		}*/
	}
	MPI_Finalize();
	return 0;
}