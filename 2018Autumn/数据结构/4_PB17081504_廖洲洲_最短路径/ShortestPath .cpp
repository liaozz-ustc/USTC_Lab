#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<process.h>

#define max 100000
int *vexs;//顶点向量
int **arc;//邻接矩阵
int vexnum;//顶点数
int startvex,endvex;
void CreatGraph(){

	printf("请输入顶点数：\n");
	scanf("%d",&vexnum);

	arc=(int **)malloc(vexnum*sizeof(int *));
	for(int i=0;i<vexnum;i++)
		arc[i]=(int *)malloc(vexnum*sizeof(int));

	printf("请输入路径:\n");
	
	for(int i=0;i<vexnum;i++)
		for(int j=0;j<vexnum;j++)
			scanf("%d",&arc[i][j]);

}
void ShortestPath(){
	int *final,*D;
	int **path;
	final=(int *)malloc(vexnum*sizeof(int));  //final[i]为1，说明已求得从startvex到vi的最短路径
	D=(int *)malloc(vexnum*sizeof(int));      //D[i]中存储了从startvex到vi的路径长度
	
	path=(int **)malloc(vexnum*sizeof(int *));
	for(int i=0;i<vexnum;i++)
		path[i]=(int *)malloc(vexnum*sizeof(int));//path[v]存储了从起点到v的最短路径

	for(int i=0;i<vexnum;i++){
		final[i]=0;
		if(i>startvex)
			D[i]=arc[startvex][i];
		else
			D[i]=arc[i][startvex];
		for(int w=1;w<vexnum;w++)
			path[i][w]=-1;
//		if(D[i]>=0){
//			path[i][startvex]=1;
//			path[i][i]=1;
		path[i][0]=startvex;

	}//for
	int w,v,min;int j;
	D[startvex]=0;final[startvex]=1;  //开始时，vo属于S
	int flag=0;//表示未找到到终点得路径
	//每次求得startvex到某个v顶点的最短路径，并加入S集
	for(int i=1;i<vexnum&&flag==0;i++){
		min = max;
		for(w=0;w<vexnum;w++){  //找出当前所知S中离起点最近的点
			if(!final[w])
				if(D[w]>0&&D[w]<min){
					v=w;
					min=D[w];
				}
		}
	final[v]=1;
	for( j=0;path[v][j]!=-1;j++);
	path[v][j]=v;
	int m,n;
	for(w=0;w<vexnum;w++){//更新最短路径
		if(v<w){
			m=v;n=w;
		}
		else{
			m=w;n=v;
		}
		if(!final[w]&&arc[m][n]>0&& ( (min+arc[m][n])<D[w] || D[w]<0 ) ) {  //如果发现有更短的路径长度，修改D[w]
			D[w]=min+arc[m][n];
		//	for(int j=0;j<vexnum;j++)
		//		path[w][j]=path[v][j];
		//	path[w][w]=1;
			for(int j=0;path[v][j]!=-1;j++)
				path[w][j]=path[v][j];
		}

	}
	if(v==endvex)
		flag=1;
	}
	printf("最短路径查找完成！\n");
	printf("长度为：%d\n",D[endvex]);
	printf("路径为：\n");
	for(int j=0;path[endvex][j]!=-1;j++)
		printf("%d  ",path[endvex][j]+1);

}

main(){
	int start,end;
	int flag;

	CreatGraph();
	do{
	printf("请输入起点和终点：\n");
	scanf("%d%d",&start,&end);
	startvex=start-1;
	endvex=end-1;
	ShortestPath();
	printf("\n继续——1，退出——0！\n");
	scanf("%d",&flag);
	}while(flag);
	

}
