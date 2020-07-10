#include <bits/stdc++.h>

using namespace std;

char outFileName[25];
int num[9][9];//棋盘
int nodes = 0;
int domain[9][9][10];//每个格子的值域，domain[i][j][0]表示当前节点取值范围，domain[i][j][k]为1表示对(i,j)k是其合法赋值

void setCell(int x,int y, int number){
    //将num[x][y]赋为number，并调整domain
    num[x][y]=number;
    int i, j;
    int cellX = (x / 3) * 3, cellY = (y / 3) * 3;//粗线宫左上角格点坐标
    for(i=0;i<9;i++){
        //对domain进行调整
        domain[x][i][number]=0;
        domain[i][y][number]=0;
    }
    if (x == y) {
        for (i = 0, j = 0; i < 9; i++, j++) {
            domain[i][j][number]=0;
        }
    }
    if (x + y == 8) {
        for (i = 0; i < 9; i++) {
            domain[i][8-i][number]=0;
        }
    }
    for (i = cellX; i < cellX + 3; i++) {
        for (j = cellY; j < cellY + 3; j++) {
            domain[i][j][number]=0;
        }
    }
}


int backtrack(int t) {
    //回溯搜索法，搜索到树的第t层，由第t层向第t+1层扩展

    if(t==81)  {
        FILE *fp;
        if ((fp = fopen(outFileName, "w")) == NULL) {
            printf("ERROR:can't create file!");
            exit(0);
        }
        for (int i = 0; i < 9; i++) {
            for (int j = 0; j < 9; j++) {
                fprintf(fp,"%d ", num[i][j]);
            }
            fprintf(fp,"\n");
        }
        //printf("  %d  \n",nodes);
        return 1;
    }

    int x=t/9;
    int y=t%9;
    if(num[x][y]!=0){
        nodes++;
        if(backtrack(t+1)){
            return 1;
        }
    }else {
        int tmp[9][9][10];
        memcpy(tmp, domain, sizeof(domain));
        for (int i = 1; i < 10; i++) {
            if (domain[x][y][i] == 1) {
                nodes++;
                setCell(x, y, i);
                if (backtrack(t+1)) {
                    return 1;
                }
                memcpy(domain, tmp, sizeof(domain));
                num[x][y]=0;
            }
        }
    }
    return 0;
}


int main(int argc, char *argv[]) {
    if (argc == 2) {


        FILE *fp;int i,j,k;
        if ((fp = fopen(argv[1], "r")) == NULL) {
            printf("ERROR:can't find %s", argv[1]);
            exit(0);
        }
        strcat(outFileName, strrchr(argv[1],'\\')+1);

        clock_t timerBegin = clock();

        for(i=0;i<9;i++){
            for(j=0;j<9;j++){
                for(k=0;k<10;k++){
                    if(k==0){
                        domain[i][j][k]=9;
                    } else{
                        domain[i][j][k]=1;
                    }
                }
            }
        }

        for (i = 0; i < 9; i++) {
            for (j = 0; j < 9; j++) {
                fscanf(fp, "%d", &num[i][j]);
                if(num[i][j])
                    setCell(i,j,num[i][j]);
            }
        }

        backtrack(0);

        clock_t timerEnd = clock();
        double_t time = (double)(timerEnd - timerBegin) / CLOCKS_PER_SEC;
        printf("time:%fs,nodes:%d\n", time, nodes);
    }
    return 0;
}