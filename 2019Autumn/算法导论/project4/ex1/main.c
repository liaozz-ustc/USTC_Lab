#include <stdio.h>
#include <stdlib.h>
int min_time=999999;//任务结束的最早时间
int *best_scheduler;//best_scheduler是一个数组，表示第i个任务由机器best_scheduler[i]执行
int n;//任务数量
int k;//机器数量
int *time;//time是一个数组，time[i]表示机器完成任务i所需时间
int *x;//x数组用于存储当前路径

int time_scheduler(int m){//执行任务所需时间
    int i,max=0;
    int *machine_time;//每台机器所花费的时间，花费最多的时间值即为任务的时间
    machine_time=(int *)malloc((k+1)* sizeof(int));
    for(i=1;i<=k;i++)
        machine_time[i]=0;
    for(i=1;i<=m;i++){
        machine_time[x[i]]=machine_time[x[i]]+time[i];
    }//将完成任务i所需时间加到完成任务i的机器的花费时间上
    for(i=1;i<=k;i++){//找出机器所花时间的最大值，即为总时间
        if(machine_time[i]>max)
            max=machine_time[i];
    }
    return max;
}

void Backtrack(int t){//子集树的回溯法，搜索到树的第t层，第i层的结点值x[i]表示第i个任务由机器x[i]执行
    //由第t层向第t+1层扩展，确定x[t]的值
    int i;
    if(t>n){//说明是当前的最优解，将该解保存下来
        min_time=time_scheduler(n);//当前任务调度所需时间
        for(i=1;i<=n;i++)
            best_scheduler[i]=x[i];
    }
    else{
        for(i=1;i<=k;i++){//对每个任务，都有k种分配方法
            x[t]=i;
            if(time_scheduler(t)<min_time)//如果当前路径的时间已经超过了最小时间，则不再往前走
                Backtrack(t+1);//向下一层扩展
        }
    }
}

int main(){
    int i;

    printf("请输入任务数量：\n");
    scanf("%d",&n);
    printf("请输入机器数量：\n");
    scanf("%d",&k);
    best_scheduler=(int *)malloc((n+1)* sizeof(int));
    time=(int *)malloc((n+1)* sizeof(int));
    x=(int *)malloc((n+1)* sizeof(int));
    printf("请输入各个任务执行时间：\n");
    for(i=1;i<=n;i++)
        scanf("%d",&time[i]);
    printf("任务数：%d,机器数：%d\n各任务时间：",n,k);
    for(i=1;i<=n;i++)
        printf("%d\t",time[i]);


    Backtrack(1);//从第一层开始，子集树回溯算法

    printf("\n最佳任务调度：\n");
    printf("任务\t");
    for(i=1;i<=n;i++)
        printf("%d\t",i);
    printf("\n机器\t");
    for(i=1;i<=n;i++)
        printf("%d\t",best_scheduler[i]);
    printf("\n完成时间：%d",min_time);
}