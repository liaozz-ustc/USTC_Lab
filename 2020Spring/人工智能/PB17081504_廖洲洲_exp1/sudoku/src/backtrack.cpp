#include <bits/stdc++.h>

using namespace std;

char outFileName[25];
int num[9][9];
int nodes = 0;

bool check(int x, int y) {//对当前状态进行检查
    int i, j;
    int cellX = (x / 3) * 3, cellY = (y / 3) * 3;//粗线宫左上角格点坐标
    for (i = 0; i < 9; i++) {
        if (num[x][i] == num[x][y] && i != y) {
            return false;
        }
    }
    for (i = 0; i < 9; i++) {
        if (num[i][y] == num[x][y] && i != x) {
            return false;
        }
    }
    if (x == y) {
        for (i = 0, j = 0; i < 9; i++, j++) {
            if (num[x][y] == num[i][j] && i != x) {
                return false;
            }
        }
    }
    if (x + y == 8) {
        for (i = 0; i < 9; i++) {
            if (num[x][y] == num[i][8 - i] && x != i) {
                return false;
            }
        }
    }
    for (i = cellX; i < cellX + 3; i++) {
        for (j = cellY; j < cellY + 3; j++) {
            if (x != i || y != j) {
                if (num[i][j] == num[x][y]) {
                    return false;
                }
            }
        }
    }
    return true;
}

int backtrack(int t) {
    //回溯搜索法，搜索到树的第t层，由第t层向第t+1层扩展
    if (t == 81) {
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
        return 1;
    } else {
        int x = t / 9;
        int y = t % 9;

        if (num[x][y] != 0) {
            nodes++;
            if (backtrack(t + 1))
                return 1;
        } else {
            for (int i = 1; i <= 9; i++) {
                num[x][y] = i;
                nodes++;
                if (check(x, y)) {
                    if (backtrack(t + 1))
                        return 1;
                }
                num[x][y] = 0;
            }
        }
    }
    return 0;
}


int main(int argc, char *argv[]) {
    if (argc == 2) {
        FILE *fp;
        if ((fp = fopen(argv[1], "r")) == NULL) {
            printf("ERROR:can't find %s", argv[1]);
            exit(0);
        }
        strcat(outFileName, strrchr(argv[1],'\\')+1);
        for (int i = 0; i < 9; i++) {
            for (int j = 0; j < 9; j++) {
                fscanf(fp, "%d", &num[i][j]);
            }
        }
        clock_t timerBegin = clock();
        backtrack(0);
        clock_t timerEnd = clock();
        double_t time = (double)(timerEnd - timerBegin) / CLOCKS_PER_SEC;
        printf("time:%fs,nodes:%d\n", time, nodes);
    }
    return 0;
}